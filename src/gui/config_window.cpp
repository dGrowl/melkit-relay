#include <algorithm>
#include <format>
#include <functional>
#include <ranges>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_pixels.h>

#include "imgui/imgui.h"

#include "gui/config_window.hpp"
#include "gui/fonts.hpp"
#include "gui/icon.hpp"
#include "gui/image.hpp"
#include "math/formula.hpp"
#include "vts/parameter.hpp"
#include "vts/request.hpp"

static constexpr auto WINDOW_TITLE = "Relay: Configuration";

static constexpr SDL_WindowFlags WINDOW_FLAGS =
    SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;

static constexpr int WINDOW_WIDTH_DEFAULT  = 960;
static constexpr int WINDOW_HEIGHT_DEFAULT = 540;
static constexpr int WINDOW_WIDTH_MIN      = 512;
static constexpr int WINDOW_HEIGHT_MIN     = 256;

static const SDL_FColor CLEAR_COLOR{.03f, .02f, .04f, 1.0f};

static constexpr unsigned N_INPUT_ICONS_PER_ROW = 8;

namespace gui {

static const char* STATUS_TEXT[] = {
    "DISCONNECTED",
    "CONNECTING",
    "FAILED",
    "CONNECTED",
};

bool ConfigWindow::showCreateParameter() {
	if (ImGui::Selectable("Create")) {
		_editingParameter = vts::Parameter();
		_editParameterModal.refresh();
		return true;
	}

	return false;
}

void ConfigWindow::showParameterControls() {
	static char filterBuffer[33]   = "";
	bool        shouldOpenEditMenu = false;
	if (ImGui::BeginTable("Parameter Top Row", 2)) {
		ImGui::TableSetupColumn("Filter", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Edit", ImGuiTableColumnFlags_WidthFixed);

		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputText("##filter", filterBuffer, IM_ARRAYSIZE(filterBuffer));

		ImGui::TableNextColumn();
		if (ImGui::Button("Edit", ImVec2(128.0f, 0.0f))) {
			shouldOpenEditMenu = true;
		}
		ImGui::EndTable();
	}
	if (shouldOpenEditMenu) {
		ImGui::OpenPopup("Edit Menu");
	}
	bool shouldOpenCreate   = false;
	bool shouldOpenDelete   = false;
	bool shouldOpenTemplate = false;
	if (ImGui::BeginPopupContextItem("Edit Menu")) {
		shouldOpenCreate   = showCreateParameter();
		shouldOpenTemplate = ImGui::Selectable("Template");
		shouldOpenDelete =
		    ImGui::Selectable("Delete",
		                      false,
		                      ImGuiSelectableFlags_None,
		                      ImVec2(128.0f, 0.0f));  // sets the width of the menu

		ImGui::EndPopup();
	}
	if (shouldOpenCreate) {
		ImGui::OpenPopup(EditParameterModal::NAME);
	}
	if (shouldOpenTemplate) {
		ImGui::OpenPopup(ParameterTemplateModal::NAME);
	}
	if (shouldOpenDelete) {
		ImGui::OpenPopup(DeleteParametersModal::NAME);
	}
}

void ConfigWindow::showParameterData() {
	bool shouldOpenModal = false;

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(12.0f, 2.0f));

	if (ImGui::BeginTable("Parameter Table",
	                      3,
	                      ImGuiTableFlags_PadOuterX
	                          | ImGuiTableFlags_RowBg
	                          | ImGuiTableFlags_ScrollY
	                          | ImGuiTableFlags_SizingFixedFit,
	                      ImVec2(0.0f, 0.0f))) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (auto& p : _paramManager.values()) {
			ImGui::PushID(p.getName().c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + 9.0f));
			if (ImGui::Button(p.getName().c_str(), ImVec2(-1.0f, 0.0f))) {
				_editingParameter = p;
				_editParameterModal.refresh();
				shouldOpenModal = true;
			}

			unsigned iIcon = 0;
			ImGui::TableNextColumn();
			for (const auto& input : p.getInputs() | std::views::values) {
				const float alpha =
				    math::remapLinear(input.getValue(), -1.0f, 1.0f, 0.1f, 1.0f);
				if (iIcon == N_INPUT_ICONS_PER_ROW) {
					iIcon = 0;
					ImGui::NewLine();
				}
				drawIcon(input.getId(), alpha);
				iIcon += 1;
			}

			ImGui::TableNextColumn();
			cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + 8.0f));
			ImGui::Text("%.2f", p.getOutput());

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	ImGui::PopStyleVar();

	if (shouldOpenModal) {
		ImGui::OpenPopup(EditParameterModal::NAME);
	}
}

void ConfigWindow::showParameterPanel() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Parameters");
	}
	if (_wsController.getStatus() == ws::Status::CONNECTED) {
		showParameters();
	}
	else {
		ImGui::Text("You'll need to connect to VTS to manage parameters.");
	}
}

void ConfigWindow::showParameters() {
	showParameterControls();
	ImGui::Separator();
	showParameterData();
}

void ConfigWindow::showGamepadSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Controller");
	}

	if (ImGui::BeginTable("Gamepad Settings", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Device");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		if (_gamepadSelector.show()) {
			_gamepadManager.setActive(_gamepadSelector.getIndex());
		}

		ImGui::EndTable();
	}

	if (ImGui::Button("Refresh", ImVec2(-1.0f, 0.0f))) {
		_gamepadManager.refreshDevices();
	}
}

void ConfigWindow::showSettingsPanel() {
	showVtsSettings();
	showGamepadSettings();
}

void ConfigWindow::showVtsSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("VTS Connection");
	}

	if (ImGui::BeginTable("VTS Config", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("API Address");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		if (ImGui::InputText("##vts-url", _urlBuffer, IM_ARRAYSIZE(_urlBuffer))) {
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

ConfigWindow::ConfigWindow(pad::Manager&          gamepadManager,
                           ws::IController&       wsController,
                           vts::ParameterManager& paramManager) :
    _gamepadManager(gamepadManager),
    _editingParameter(paramManager.getSample()),
    _paramManager(paramManager),
    _wsController(wsController),
    _newParamNameBuffer(),
    _urlBuffer(),
    _gamepadSelector("##active-gamepad", _gamepadManager.getNames()),
    _window(nullptr),
    _deleteParametersModal(_paramManager, wsController),
    _editParameterModal(wsController, _editingParameter),
    _parameterTemplateModal(wsController) {
	SDL_strlcpy(_urlBuffer, wsController.getUrl(), sizeof(_urlBuffer));
}

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
	Surface icon(ICON_PATH);
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

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoResize
	                               | ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (ImGui::Begin("Settings", nullptr, windowFlags)) {
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		if (ImGui::BeginTable("Panels",
		                      2,
		                      ImGuiTableFlags_Resizable,
		                      contentRegion)) {
			ImGui::TableNextColumn();
			showSettingsPanel();

			ImGui::TableNextColumn();
			showParameterPanel();

			showParameterModals();

			ImGui::EndTable();
		}
	}
	ImGui::End();

	ImGui::Render();
	auto       drawData = ImGui::GetDrawData();
	const bool isMinimized =
	    (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);

	auto            commandBuffer    = SDL_AcquireGPUCommandBuffer(gpu);
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

		auto renderPass =
		    SDL_BeginGPURenderPass(commandBuffer, &targetInfo, 1, nullptr);
		ImGui_ImplSDLGPU3_RenderDrawData(drawData, commandBuffer, renderPass);
		SDL_EndGPURenderPass(renderPass);
	}

	SDL_SubmitGPUCommandBuffer(commandBuffer);
}

void ConfigWindow::setActiveGamepadIndex(const size_t activeIndex) {
	_gamepadSelector.setIndex(activeIndex);
}

void ConfigWindow::showParameterModals() {
	_deleteParametersModal.show();
	_editParameterModal.show();
	_parameterTemplateModal.show();
}

}  // namespace gui
