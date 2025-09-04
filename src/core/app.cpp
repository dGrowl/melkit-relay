#include <imgui/imgui.h>

#include "core/app.hpp"
#include "core/settings.hpp"
#include "mnk/event.hpp"
#include "vts/request.hpp"
#include "vts/response.hpp"
#include "ws/event.hpp"

namespace core {

App::App() :
    _alive(true),
    _gpu(nullptr),
    _params(),
    _mnkMonitor(),
    _wsClient(),
    _config(_wsClient, _params),
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
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		return 1;
	}

	_gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV
	                               | SDL_GPU_SHADERFORMAT_DXIL
	                               | SDL_GPU_SHADERFORMAT_METALLIB,
	                           true,
	                           nullptr);
	if (_gpu == nullptr) {
		return 2;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();

	_icon.setConfigCallback(App::openConfigCallback, this);
	_icon.setQuitCallback(App::quitCallback, this);

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
		while (SDL_PollEvent(&event)) {
			handleEvent(event);
		}
		_config.render(_gpu);

		_params.update();
		SDL_Delay(10);
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
			_params.handleEvent(event.user);
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
	}
}

void App::handleVtsAuthenticationToken(SDL_UserEvent& event) {
	auto* data = static_cast<vts::AuthenticationTokenData*>(event.data1);
	SETTINGS.setAuthToken(data->token.c_str());
	vts::authenticate(_wsClient);
	delete data;
}

void App::handleVtsAuthenticationSuccess() {
	vts::getParameters(_wsClient);
}

void App::handleVtsAuthenticationFailure() {
	SETTINGS.setAuthToken("");
	stopWs();
}

void App::handleVtsInputParameterList(SDL_UserEvent& event) {
	auto* data = static_cast<vts::InputParameterListData*>(event.data1);
	_params.clear();
	for (const auto& p : data->parameters) {
		_params.add(p);
	}
	delete data;
	loadParameterSettings();
}

void App::handleVtsParameterCreation() {
	vts::getParameters(_wsClient);
}

void App::handleVtsParameterDeletion() {
	vts::getParameters(_wsClient);
}

void App::loadParameterSettings() {
	auto settingsParameters = SETTINGS.getParameters();
	for (const auto& settingsParameter : settingsParameters) {
		auto it = _params.find(settingsParameter.name);
		if (it == _params.end()) {
			SETTINGS.removeParameter(settingsParameter.name);
		}
		else {
			it->second.setInputs(settingsParameter.inputs);
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
