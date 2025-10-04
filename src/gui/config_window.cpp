#include "gui/config_window.hpp"

#include <cstddef>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_video.h>

#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_sdlgpu3.h"
#include "imgui/imgui.h"

#include "gui/image.hpp"
#include "impulse/processor.hpp"
#include "pad/manager.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

static constexpr auto WINDOW_TITLE = "Relay: Configuration";

static constexpr SDL_WindowFlags WINDOW_FLAGS =
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

static constexpr int WINDOW_WIDTH_DEFAULT  = 960;
static constexpr int WINDOW_HEIGHT_DEFAULT = 540;
static constexpr int WINDOW_WIDTH_MIN      = 512;
static constexpr int WINDOW_HEIGHT_MIN     = 256;

static const SDL_FColor CLEAR_COLOR{.03f, .02f, .04f, 1.0F};

namespace gui {

void ConfigWindow::showMenuBar() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Menu")) {
			if (ImGui::MenuItem("Quit")) {
				SDL_Event quitEvent{.type = SDL_EVENT_QUIT};
				SDL_PushEvent(&quitEvent);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

ConfigWindow::ConfigWindow(pad::Manager&          gamepadManager,
                           imp::Processor&        impulseProcessor,
                           ws::IController&       wsController,
                           vts::ParameterManager& paramManager) :
    _window(nullptr),
    _settingsPanel(gamepadManager,
                   impulseProcessor,
                   paramManager.getSample(),
                   wsController),
    _parameterPanel(paramManager.getSample(), paramManager, wsController) {}

bool ConfigWindow::isOpen() const {
	return _window != nullptr;
}

int ConfigWindow::open(SDL_GPUDevice* gpu) {
	if (_window != nullptr) {
		SDL_RaiseWindow(_window);
		return 0;
	}

	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const float         mainScale = SDL_GetDisplayContentScale(primaryDisplay);

	_window = SDL_CreateWindow(WINDOW_TITLE,
	                           static_cast<int>(WINDOW_WIDTH_DEFAULT * mainScale),
	                           static_cast<int>(WINDOW_HEIGHT_DEFAULT * mainScale),
	                           WINDOW_FLAGS);
	if (_window == nullptr) {
		return 1;
	}

	SDL_SetWindowMinimumSize(_window, WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);
	SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	const Surface icon(ICON_PATH);
	SDL_SetWindowIcon(_window, icon);
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
	initInfo.Device                     = gpu;
	initInfo.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(gpu, _window);
	initInfo.MSAASamples       = SDL_GPU_SAMPLECOUNT_1;
	ImGui_ImplSDLGPU3_Init(&initInfo);

	return 0;
}

SDL_WindowID ConfigWindow::id() const {
	return SDL_GetWindowID(_window);
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

	const ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar
	                                     | ImGuiWindowFlags_NoDecoration
	                                     | ImGuiWindowFlags_NoMove
	                                     | ImGuiWindowFlags_NoResize
	                                     | ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (ImGui::Begin("Config", nullptr, windowFlags)) {
		showMenuBar();
		const ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("Panels",
		                      2,
		                      ImGuiTableFlags_Resizable,
		                      contentRegion)) {
			ImGui::TableNextColumn();
			_settingsPanel.show();

			ImGui::TableNextColumn();
			_parameterPanel.show();

			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::Render();
	auto*      drawData = ImGui::GetDrawData();
	const bool isMinimized =
	    (drawData->DisplaySize.x <= 0.0F || drawData->DisplaySize.y <= 0.0F);

	auto*           commandBuffer    = SDL_AcquireGPUCommandBuffer(gpu);
	SDL_GPUTexture* swapchainTexture = nullptr;
	SDL_WaitAndAcquireGPUSwapchainTexture(commandBuffer,
	                                      _window,
	                                      &swapchainTexture,
	                                      nullptr,
	                                      nullptr);

	if (swapchainTexture != nullptr && !isMinimized) {
		ImGui_ImplSDLGPU3_PrepareDrawData(drawData, commandBuffer);

		SDL_GPUColorTargetInfo targetInfo = {};
		targetInfo.texture                = swapchainTexture;
		targetInfo.clear_color            = CLEAR_COLOR;
		targetInfo.load_op                = SDL_GPU_LOADOP_CLEAR;
		targetInfo.store_op               = SDL_GPU_STOREOP_STORE;
		targetInfo.mip_level              = 0;
		targetInfo.layer_or_depth_plane   = 0;
		targetInfo.cycle                  = false;

		auto* renderPass =
		    SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(commandBuffer);
}

void ConfigWindow::setActiveGamepadIndex(const size_t activeIndex) {
	_settingsPanel.setActiveGamepadIndex(activeIndex);
}

}  // namespace gui
