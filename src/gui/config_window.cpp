#include <format>

#include <SDL3/SDL_events.h>

#include "imgui/imgui.h"

#include "gui/config_window.hpp"
#include "vts/request.hpp"

namespace gui {

static const char* STATUS_TEXT[] = {
    "DISCONNECTED",
    "CONNECTING",
    "FAILED",
    "CONNECTED",
};

ConfigWindow::ConfigWindow(ws::IController& wsController,
                           vts::ParameterManager& paramManager) :
    _title("Configuration"),
    _height(540),
    _width(960),
    _clearColor{.03f, .02f, .04f, 1.0f},
    _paramManager(paramManager),
    _wsController(wsController),
    _urlBuffer(),
    _newParamNameBuffer(),
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
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("Columns",
		                      2,
		                      ImGuiTableFlags_Resizable,
		                      contentRegion)) {
			ImGui::TableNextColumn();
			showVtsConnection();

			ImGui::TableNextColumn();
			showParameters();

			ImGui::EndTable();
		}
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
	ImGui::Text("Settings");

	ImGui::SeparatorText("VTS Connection");

	if (ImGui::BeginTable("VTS Config", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("API Address");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		if (ImGui::InputText("##vtsUrl", _urlBuffer, IM_ARRAYSIZE(_urlBuffer))) {
			_wsController.setUrl(_urlBuffer);
		}
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Status");
		ImGui::TableNextColumn();
		ImGui::Text(STATUS_TEXT[_wsController.getStatus()]);

		ImGui::EndTable();
	}

	if (_wsController.getStatus() == ws::Status::CONNECTED) {
		if (ImGui::Button("Disconnect", ImVec2(-1.0f, 0.0f))) {
			_wsController.stop();
		}
	}
	else {
		if (ImGui::Button("Connect", ImVec2(-1.0f, 0.0f))) {
			_wsController.setUrl(_urlBuffer);
			_wsController.start();
		}
	}
}

void ConfigWindow::showParameters() {
	ImGui::Text("Parameters");
	ImGui::Separator();

	if (ImGui::Button("Add", ImVec2(-1.0f, 0.0f))) {
		ImGui::OpenPopup("Create Parameter");
	}

	if (ImGui::BeginPopupModal("Create Parameter",
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::SeparatorText("Details");
		ImGui::Text("MK_");
		ImGui::SameLine();
		ImGui::InputText("Name",
		                 _newParamNameBuffer,
		                 IM_ARRAYSIZE(_newParamNameBuffer));

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			vts::ParameterData newParam{
			    .name = std::format("MK_{}", _newParamNameBuffer),
			    .defaultValue = 0.0f,
			    .max = 1.0f,
			    .min = 0.0f,
			};
			_paramManager.add(newParam);
			vts::createParameter(_wsController, newParam);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	for (const auto& p : _paramManager.values()) {
		ImGui::Button(p.getName().c_str(), ImVec2(-1.0f, 0.0f));
	}
}

}  // namespace gui
