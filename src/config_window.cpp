#include "config_window.hpp"

const char* STATUS_TEXT[] = {
    "DISCONNECTED",
    "CONNECTING",
    "FAILED",
    "CONNECTED",
};

ConfigWindow::ConfigWindow(ws::IController& wsController) :
    _title("Configuration"),
    _height(540),
    _width(960),
    _clearColor{.03f, .02f, .04f, 1.0f},
    _wsController(wsController),
    _urlBuffer(),
    _window(nullptr),
    _flags(SDL_WINDOW_RESIZABLE
           | SDL_WINDOW_HIDDEN
           | SDL_WINDOW_HIGH_PIXEL_DENSITY) {
	SDL_strlcpy(_urlBuffer, wsController.getUrl(), sizeof(_urlBuffer));
}

int ConfigWindow::open(SDL_GPUDevice* gpu) {
	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const float mainScale = SDL_GetDisplayContentScale(primaryDisplay);

	_window = SDL_CreateWindow(_title,
	                           (int)(_width * mainScale),
	                           (int)(_height * mainScale),
	                           _flags);
	if (_window == nullptr) {
		return 1;
	}

	SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(_window);

	if (!SDL_ClaimWindowForGPUDevice(gpu, _window)) {
		return 2;
	}
	SDL_SetGPUSwapchainParameters(gpu,
	                              _window,
	                              SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
	                              SDL_GPU_PRESENTMODE_VSYNC);

	auto& style = ImGui::GetStyle();
	style.ScaleAllSizes(mainScale);
	style.FontScaleDpi = mainScale;

	ImGui_ImplSDL3_InitForSDLGPU(_window);
	ImGui_ImplSDLGPU3_InitInfo initInfo = {};
	initInfo.Device = gpu;
	initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu, _window);
	initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
	ImGui_ImplSDLGPU3_Init(&initInfo);

	return 0;
}

void ConfigWindow::render(SDL_GPUDevice* gpu) {
	if (_window == nullptr) {
		return;
	}

	ImGui_ImplSDLGPU3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoResize
	                               | ImGuiWindowFlags_NoBringToFrontOnFocus;

	if (ImGui::Begin("Settings", nullptr, windowFlags)) {
		showVtsConnection();
	}
	ImGui::End();

	ImGui::Render();
	auto drawData = ImGui::GetDrawData();
	const bool isMinimized =
	    (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);

	auto commandBuffer = SDL_AcquireGPUCommandBuffer(gpu);
	SDL_GPUTexture* swapchainTexture = nullptr;
	SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer,
	                                      _window,
	                                      &swapchainTexture,
	                                      nullptr,
	                                      nullptr);

	if (swapchainTexture != nullptr && !isMinimized) {
		ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

		SDL_GPUColorTargetInfo targetInfo = {};
		targetInfo.texture = swapchainTexture;
		targetInfo.clear_color = _clearColor;
		targetInfo.load_op = SDL_GPU_LOADOP_CLEAR;
		targetInfo.store_op = SDL_GPU_STOREOP_STORE;
		targetInfo.mip_level = 0;
		targetInfo.layer_or_depth_plane = 0;
		targetInfo.cycle = false;

		auto renderPass =
		    SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(commandBuffer);
}

void ConfigWindow::close(SDL_GPUDevice* gpu) {
	if (_window == nullptr) {
		return;
	}

	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();

	SDL_ReleaseWindowFromGPUDevice(gpu, _window);
	SDL_DestroyWindow(_window);

	_window = nullptr;
}

bool ConfigWindow::isOpen() const {
	return _window != nullptr;
}

SDL_WindowID ConfigWindow::id() const {
	return SDL_GetWindowID(_window);
}

void ConfigWindow::showVtsConnection() {
	ImGui::Text("VTS Connection");
	ImGui::Separator();
	if (ImGui::InputText("API Address", _urlBuffer, IM_ARRAYSIZE(_urlBuffer))) {
		_wsController.setUrl(_urlBuffer);
	}
	ImGui::Text("Status: ");
	ImGui::SameLine();
	ImGui::Text(STATUS_TEXT[_wsController.getStatus()]);

	if (_wsController.getStatus() == ws::Status::CONNECTED) {
		if (ImGui::Button("Disconnect", ImVec2(120, 0))) {
			_wsController.stop();
		}
	}
	else {
		if (ImGui::Button("Connect", ImVec2(120, 0))) {
			_wsController.setUrl(_urlBuffer);
			_wsController.start();
		}
	}
}
