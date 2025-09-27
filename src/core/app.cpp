#include "core/app.hpp"

#include <format>

#include "imgui/imgui.h"

#include "core/settings.hpp"
#include "gui/fonts.hpp"
#include "gui/theme.hpp"
#include "gui/utility.hpp"
#include "mnk/event.hpp"
#include "vts/request.hpp"
#include "vts/response.hpp"
#include "ws/event.hpp"

namespace core {

App::App() :
    _alive(true),
    _pacer(),
    _gpu(nullptr),
    _parameters(),
    _impulseProcessor(),
    _wsClient(),
    _mnkMonitor(),
    _gamepadManager(),
    _config(_gamepadManager, _impulseProcessor, _wsClient, _parameters),
    _icon() {
	ws::allocateEvents();
	mnk::allocateEvents();
	_wsClient.start();
}

App::~App() {
	SDL_WaitForGPUIdle(_gpu);

	ImGui::DestroyContext();

	SDL_DestroyGPUDevice(_gpu);
	SDL_Quit();
}

int App::init() {
	_gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV
	                               | SDL_GPU_SHADERFORMAT_DXIL
	                               | SDL_GPU_SHADERFORMAT_METALLIB,
	                           true,
	                           nullptr);
	if (_gpu == nullptr) {
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	gui::Theme::init();
	gui::Fonts::init();

	_icon.setConfigCallback(App::openConfigCallback, this);
	_icon.setQuitCallback(App::quitCallback, this);

	openConfig();

	return 0;
}

void App::quit() {
	_config.close(_gpu);
	stopMouseKeyboard();
	stopWs();
	_alive = false;
}

void App::openConfigCallback(void* userdata, SDL_TrayEntry*) {
	App* instance = static_cast<App*>(userdata);
	if (instance != nullptr) {
		instance->openConfig();
	}
}

void App::quitCallback(void* userdata, SDL_TrayEntry*) {
	App* instance = static_cast<App*>(userdata);
	if (instance != nullptr) {
		instance->quit();
	}
}

void App::run() {
	SDL_Event event;
	while (_alive) {
		_pacer.startFrame();

		while (SDL_PollEvent(&event)) {
			handleEvent(event);
		}
		_config.render(_gpu);

		_impulseProcessor.update();
		for (const auto& [id, value] : _impulseProcessor.impulses()) {
			_parameters.distributeImpulse(id, value);
		}
		checkParameterValues();
		_impulseProcessor.clear();

		_pacer.endFrame();
	}
}

void App::openConfig() {
	_config.open(_gpu);
}

void App::stopMouseKeyboard() {
	_mnkMonitor.stop();
}

void App::stopWs() {
	_wsClient.stop();
}

void App::handleEvent(SDL_Event& event) {
	if (_config.isOpen()) {
		ImGui_ImplSDL3_ProcessEvent(&event);
	}
	switch (event.type) {
		case mnk::Event::INPUT:
			_impulseProcessor.handleEvent(event.user);
			break;
		case ws::Event::OPEN:
			vts::authenticate(_wsClient);
			break;
		case ws::Event::MESSAGE:
			handleVtsMessage(event.user);
			break;
		case SDL_EVENT_QUIT:
			quit();
			break;
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			handleWindowClose(event);
			break;
		case SDL_EVENT_GAMEPAD_ADDED:
			_gamepadManager.refreshDevices();
			_config.setActiveGamepadIndex(_gamepadManager.getActiveIndex());
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			_gamepadManager.clearActive();
			_gamepadManager.refreshDevices();
			_config.setActiveGamepadIndex(_gamepadManager.getActiveIndex());
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			_impulseProcessor.handleGamepadEvent(event, _gamepadManager.getActiveId());
			break;
	}
}

void App::handleVtsAuthenticationToken(SDL_UserEvent& event) {
	auto* token = static_cast<std::string*>(event.data1);
	SETTINGS.setAuthToken(token->c_str());
	vts::authenticate(_wsClient);
	delete token;
}

void App::handleVtsAuthenticationSuccess() {
	vts::getParameters(_wsClient);
}

void App::handleVtsAuthenticationFailure() {
	SETTINGS.setAuthToken("");
	stopWs();
}

void App::handleVtsInputParameterList(SDL_UserEvent& event) {
	auto* names = static_cast<std::vector<std::string>*>(event.data1);
	_parameters.clear();
	for (const auto& name : *names) {
		_parameters.add(name);
	}
	delete names;
	loadParameterSettings();
}

void App::handleVtsParameterCreation() {
	vts::getParameters(_wsClient);
}

void App::handleVtsParameterDeletion() {
	vts::getParameters(_wsClient);
}

void App::checkParameterValues() {
	std::vector<vts::ParameterValue> payload;
	for (auto& parameter : _parameters.values()) {
		if (parameter.isFresh()) {
			payload.emplace_back(parameter.getName(), parameter.getOutput());
		}
	}
	if (!payload.empty()) {
		vts::setParameters(_wsClient, payload);
	}
}

void App::loadParameterSettings() {
	auto settingsParameters = SETTINGS.getParameters();
	for (const auto& settingsParameter : settingsParameters) {
		auto it = _parameters.find(settingsParameter.name);
		if (it == _parameters.end()) {
			SETTINGS.removeParameter(settingsParameter.name);
		}
		else {
			it->second.setBlendMode(settingsParameter.blendMode);
			for (const auto& input : settingsParameter.inputs) {
				it->second.addInput(input.id, input.isInverted);
			}
		}
	}
}

void App::handleVtsMessage(SDL_UserEvent& event) {
	switch (event.code) {
		case vts::ResponseCode::AUTHENTICATION_TOKEN:
			handleVtsAuthenticationToken(event);
			break;
		case vts::ResponseCode::AUTHENTICATION_SUCCESS:
			handleVtsAuthenticationSuccess();
			break;
		case vts::ResponseCode::AUTHENTICATION_FAILURE:
			handleVtsAuthenticationFailure();
			break;
		case vts::ResponseCode::INPUT_PARAMETER_LIST:
			handleVtsInputParameterList(event);
			break;
		case vts::ResponseCode::PARAMETER_CREATION:
			handleVtsParameterCreation();
			break;
		case vts::ResponseCode::PARAMETER_DELETION:
			handleVtsParameterDeletion();
			break;
	}
}

void App::handleWindowClose(SDL_Event& event) {
	if (event.window.windowID == _config.id()) {
		_config.close(_gpu);
	}
}

}  // namespace core
