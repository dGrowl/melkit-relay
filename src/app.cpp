#include "app.hpp"

App::App() :
    _alive(true),
    _gpu(nullptr),
    _uioThread(nullptr),
    _config(),
    _icon() {
	_uioThread = SDL_CreateThread(uioHookThreadFn, "uiohook", nullptr);
	allocateUIOEvents();
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
	stopInput();
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

		SDL_Delay(10);
	}
}

void App::openConfig() {
	_config.open(_gpu);
}

void App::stopInput() {
	hook_stop();
	SDL_WaitThread(_uioThread, nullptr);
	_uioThread = nullptr;
}

void App::handleEvent(SDL_Event& event) {
	if (_config.isOpen()) {
		ImGui_ImplSDL3_ProcessEvent(&event);
	}
	if (event.type == UIO_EVENT_KEY_DOWN) {
		auto* e = static_cast<UIO_Event*>(event.user.data1);
		// handle keydown
		SDL_free(e);
	} else if (event.type == UIO_EVENT_MOUSE_MOVE) {
		auto* e = static_cast<UIO_Event*>(event.user.data1);
		// handle mousemove
		SDL_free(e);
	} else if (event.type == SDL_EVENT_QUIT) {
		quit();
	} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
	           event.window.windowID == _config.id()) {
		_config.close(_gpu);
	}
}
