#include <SDL3/SDL.h>

#include "../lib/imgui/backends/imgui_impl_sdl3.h"
#include "../lib/imgui/backends/imgui_impl_sdlgpu3.h"
#include "../lib/imgui/imgui.h"

const SDL_InitFlags SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

const char* WINDOW_TITLE = "SDL3 Starter";
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const SDL_WindowFlags WINDOW_FLAGS =
    (SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);

int main(int, char*[]) {
	if (!SDL_Init(SDL_FLAGS)) {
		return 1;
	}

	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const float mainScale = SDL_GetDisplayContentScale(primaryDisplay);
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE,
	                                      (int)(WINDOW_WIDTH * mainScale),
	                                      (int)(WINDOW_HEIGHT * mainScale),
	                                      WINDOW_FLAGS);
	if (window == nullptr) {
		return 2;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	SDL_GPUDevice* gpuDevice = SDL_CreateGPUDevice(
	    SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL |
	        SDL_GPU_SHADERFORMAT_METALLIB,
	    true,
	    nullptr);
	if (gpuDevice == nullptr) {
		return 3;
	}

	if (!SDL_ClaimWindowForGPUDevice(gpuDevice, window)) {
		return 4;
	}
	SDL_SetGPUSwapchainParameters(gpuDevice,
	                              window,
	                              SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
	                              SDL_GPU_PRESENTMODE_VSYNC);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();
	style.ScaleAllSizes(mainScale);
	style.FontScaleDpi = mainScale;

	ImGui_ImplSDL3_InitForSDLGPU(window);
	ImGui_ImplSDLGPU3_InitInfo initInfo = {};
	initInfo.Device = gpuDevice;
	initInfo.ColorTargetFormat =
	    SDL_GetGPUSwapchainTextureFormat(gpuDevice, window);
	initInfo.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
	ImGui_ImplSDLGPU3_Init(&initInfo);

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	bool alive = true;
	bool showDemoWindow = true;
	SDL_Event event;
	while (alive) {
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT) {
				alive = false;
			} else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
			           event.window.windowID == SDL_GetWindowID(window)) {
				alive = false;
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

		auto commandBuffer = SDL_AcquireGPUCommandBuffer(gpuDevice);
		SDL_GPUTexture* swapchainTexture = nullptr;
		SDL_WaitAndAcquireGPUSwapchainTexture(
		    commandBuffer, window, &swapchainTexture, nullptr, nullptr);

		if (swapchainTexture != nullptr && !isMinimized) {
			ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

			SDL_GPUColorTargetInfo targetInfo = {};
			targetInfo.texture = swapchainTexture;
			targetInfo.clear_color =
			    SDL_FColor{clearColor.x, clearColor.y, clearColor.z, clearColor.w};
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

		SDL_Delay(100);
	}

	SDL_WaitForGPUIdle(gpuDevice);
	ImGui_ImplSDL3_Shutdown();
	ImGui_ImplSDLGPU3_Shutdown();
	ImGui::DestroyContext();

	SDL_ReleaseWindowFromGPUDevice(gpuDevice, window);
	SDL_DestroyGPUDevice(gpuDevice);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
