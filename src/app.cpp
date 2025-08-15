#include "app.hpp"

App::App() :
    _alive(true),
    _input(),
    _gpu(nullptr),
    _uioThread(nullptr),
    _wsThread(nullptr),
    _config(),
    _icon() {
	_uioThread = SDL_CreateThread(uioHookThreadFn, "uio", nullptr);
	_wsThread = SDL_CreateThread(wsThreadFn, "ws", nullptr);
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
	stopUio();
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

		SDL_Delay(10);
	}
}

void App::openConfig() {
	_config.open(_gpu);
}

void App::stopUio() {
	hook_stop();
	SDL_WaitThread(_uioThread, nullptr);
	_uioThread = nullptr;
}

void App::stopWs() {
	stopWsThread();
	SDL_WaitThread(_wsThread, nullptr);
	_wsThread = nullptr;
}

void App::handleEvent(SDL_Event& event) {
	if (_config.isOpen()) {
		ImGui_ImplSDL3_ProcessEvent(&event);
	}
	switch (event.type) {
		case UIO_EVENT_KEY_DOWN:
			_input.handleKeyDown(event);
			break;
		case UIO_EVENT_KEY_UP:
			_input.handleKeyUp(event);
			break;
		case UIO_EVENT_MOUSE_MOVE:
			_input.handleMouseMove(event);
			break;
		case UIO_EVENT_MOUSE_CLICK:
			_input.handleMouseButton(event, true);
			break;
		case UIO_EVENT_MOUSE_RELEASE:
			_input.handleMouseButton(event, false);
			break;
		case SDL_EVENT_QUIT:
			quit();
			break;
		case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
			handleWindowClose(event);
			break;
	}
}

void App::handleWindowClose(SDL_Event& event) {
	if (event.window.windowID == _config.id()) {
		_config.close(_gpu);
	}
}
