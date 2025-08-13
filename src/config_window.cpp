#include <SDL3/SDL.h>

#include "../lib/imgui/backends/imgui_impl_sdl3.h"
#include "../lib/imgui/backends/imgui_impl_sdlgpu3.h"
#include "../lib/imgui/imgui.h"
#include "config_window.hpp"

ConfigWindow::ConfigWindow()
    : _alive(true),
      _title("Config"),
      _height(540),
      _width(960),
      _event(),
      _clearColor{.03f, .02f, .04f, 1.0f},
      _window(nullptr),
      _flags(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN |
             SDL_WINDOW_HIGH_PIXEL_DENSITY) {}

int ConfigWindow::create(SDL_GPUDevice* gpu) {
	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const float mainScale = SDL_GetDisplayContentScale(primaryDisplay);

	_window = SDL_CreateWindow(
	    _title, (int)(_width * mainScale), (int)(_height * mainScale), _flags);
	if (_window == nullptr) {
		return 1;
	}

	SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(_window);

	if (!SDL_ClaimWindowForGPUDevice(gpu, _window)) {
		return 2;
	}
	SDL_SetGPUSwapchainParameters(
	    gpu, _window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC);

	ImGui::CreateContext();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();
	style.ScaleAllSizes(mainScale);
	style.FontScaleDpi = mainScale;

	ImGui_ImplSDL3_InitForSDLGPU(_window);
	ImGui_ImplSDLGPU3_InitInfo initInfo = {};
	initInfo.Device = gpu;
	initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu, _window);
	initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
	ImGui_ImplSDLGPU3_Init(&initInfo);

	bool showDemoWindow = true;
	while (_alive) {
		while (SDL_PollEvent(&_event)) {
			ImGui_ImplSDL3_ProcessEvent(&_event);
			if (_event.type == SDL_EVENT_QUIT) {
				_alive = false;
			} else if (_event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
			           _event.window.windowID == SDL_GetWindowID(_window)) {
				_alive = false;
			}
		}

		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		if (showDemoWindow) {
			ImGui::ShowDemoWindow(&showDemoWindow);
		}

		ImGui::Render();
		auto drawData = ImGui::GetDrawData();
		const bool isMinimized =
		    (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);

		auto commandBuffer = SDL_AcquireGPUCommandBuffer(gpu);
		SDL_GPUTexture* swapchainTexture = nullptr;
		SDL_WaitAndAcquireGPUSwapchainTexture(
		    commandBuffer, _window, &swapchainTexture, nullptr, nullptr);

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

		SDL_Delay(10);
	}

	SDL_WaitForGPUIdle(gpu);
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();

	destroy(gpu);
	return 0;
}

void ConfigWindow::destroy(SDL_GPUDevice* gpu) {
	SDL_ReleaseWindowFromGPUDevice(gpu, _window);
	SDL_DestroyWindow(_window);
}
