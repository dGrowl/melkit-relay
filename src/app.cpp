#include "app.hpp"

App::App() :
    _alive(true),
    _gpu(nullptr),
    _config(),
    _icon() {}

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

	_gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV |
	                               SDL_GPU_SHADERFORMAT_DXIL |
	                               SDL_GPU_SHADERFORMAT_METALLIB,
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
			if (_config.isOpen()) {
				ImGui_ImplSDL3_ProcessEvent(&event);
			}
			if (event.type == SDL_EVENT_QUIT) {
				_config.close(_gpu);
			} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
			           event.window.windowID == _config.id()) {
				_config.close(_gpu);
			}
		}
		_config.render(_gpu);

		SDL_Delay(10);
	}
}

void App::openConfig() {
	_config.open(_gpu);
}
