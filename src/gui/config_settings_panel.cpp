#include "gui/config_settings_panel.hpp"

#include <cstddef>
#include <format>

#include <SDL3/SDL_stdinc.h>

#include "imgui/imgui.h"

#include "gui/fonts.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace gui {

static const char* STATUS_TEXT[] = {
    "DISCONNECTED",
    "CONNECTING",
    "FAILED",
    "CONNECTED",
};

void ConfigSettingsPanel::showGamepadSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Controller");
	}

	if (ImGui::BeginTable("GamepadSettings", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Device");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0F);
		if (_gamepadSelector.show()) {
			_gamepadManager.setActive(_gamepadSelector.getIndex());
		}

		ImGui::EndTable();
	}

	ImGui::Spacing();
}

void ConfigSettingsPanel::showMouseMotionSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Mouse (Motion)");
	}

	if (ImGui::BeginTable("MouseMotionSettings",
	                      2,
	                      ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Sensitivity");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0F);
		if (ImGui::SliderInt("##mouse-sensitivity-slider",
		                     &_mouseSensitivity,
		                     1,
		                     100)) {
			_impulseProcessor.setMouseSensitivity(_mouseSensitivity);
		}
		ImGui::SetItemTooltip(
		    "Makes mouse motion inputs more or less intense.\n\nYou can Ctrl + Click "
		    "on this slider to type in a value.");

		ImGui::EndTable();
	}

	ImGui::Spacing();
}

static constexpr float REGION_DISPLAY_HEIGHT = 64.0F;

void ConfigSettingsPanel::showMousePositionSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Mouse (Position)");
	}

	if (ImGui::BeginTable("MousePositionSettings",
	                      2,
	                      ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Region");
		ImGui::TableNextColumn();

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		const ImVec2 cursorPos      = ImGui::GetCursorScreenPos();
		const float  availableWidth = ImGui::GetContentRegionAvail().x;

		const ImVec2 rectMin = cursorPos;
		const ImVec2 rectMax(cursorPos.x + availableWidth - 1.0F,
		                     cursorPos.y + REGION_DISPLAY_HEIGHT);

		const ImU32 fillColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
		const ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

		drawList->AddRectFilled(rectMin, rectMax, fillColor);

		const auto& mouseBounds = _impulseProcessor.getMouseBounds();
		const auto  topLeftString =
		    std::format("({}, {})", mouseBounds.left, mouseBounds.top);
		const auto bottomRightString =
		    std::format("({}, {})", mouseBounds.right, mouseBounds.bottom);
		const char* topLeftText     = topLeftString.c_str();
		const char* bottomRightText = bottomRightString.c_str();

		ImVec2 topLeftTextPos = rectMin;
		topLeftTextPos.x += 3.0F;
		topLeftTextPos.y += 2.0F;

		const ImVec2 bottomRightTextSize = ImGui::CalcTextSize(bottomRightText);
		ImVec2       bottomRightTextPos  = rectMax;
		bottomRightTextPos.x -= bottomRightTextSize.x + 3.0F;
		bottomRightTextPos.y -= bottomRightTextSize.y + 2.0F;

		drawList->AddText(topLeftTextPos, textColor, topLeftText);
		drawList->AddText(bottomRightTextPos, textColor, bottomRightText);
		ImGui::SetCursorScreenPos(rectMin);
		ImGui::InvisibleButton("##rect_hover",
		                       ImVec2(rectMax.x - rectMin.x, rectMax.y - rectMin.y));
		ImGui::SetItemTooltip(
		    "This area shows the mouse tracking bounds.\n\n"
		    "When your mouse is at the bottom-left corner, its position is 0,0.\n"
		    "When your mouse is at the top-right corner, its position is 1,1.\n"
		    "Anywhere in between gives you a smooth range from 0 to 1.\n\n"
		    "You can use the Adjust button below to change these boundaries.");

		ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, rectMax.y));
		ImGui::Spacing();

		ImGui::EndTable();
	}

	if (ImGui::Button("Adjust", ImVec2(-1.0F, 0.0F))) {
		_setMouseBoundsModal.refresh();
		ImGui::OpenPopup(SetMouseBoundsModal::NAME);
	}

	ImGui::Spacing();
}

void ConfigSettingsPanel::showVtsSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("VTS Connection");
	}

	if (ImGui::BeginTable("VTSConfig", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("API Address");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0F);
		ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
		if (ImGui::InputText("##vts-url", _urlBuffer, IM_ARRAYSIZE(_urlBuffer))) {
			_wsController.setUrl(_urlBuffer);
		}
		ImGui::PopItemFlag();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Status");
		ImGui::TableNextColumn();
		ImGui::Text("%s", STATUS_TEXT[_wsController.getStatus()]);

		ImGui::EndTable();
	}

	if (_wsController.getStatus() == ws::Status::CONNECTED) {
		if (ImGui::Button("Disconnect", ImVec2(-1.0F, 0.0F))) {
			_wsController.stop();
		}
	}
	else {
		if (ImGui::Button("Connect", ImVec2(-1.0F, 0.0F))) {
			_wsController.setUrl(_urlBuffer);
			_wsController.start();
		}
	}

	ImGui::Spacing();
}

void ConfigSettingsPanel::showModals() {
	_setMouseBoundsModal.show();
}

ConfigSettingsPanel::ConfigSettingsPanel(pad::Manager&    gamepadManager,
                                         imp::Processor&  impulseProcessor,
                                         vts::Parameter&  editingParameter,
                                         ws::IController& wsController) :
    _gamepadManager(gamepadManager),
    _impulseProcessor(impulseProcessor),
    _wsController(wsController),
    _setMouseBoundsModal(impulseProcessor, editingParameter),
    _urlBuffer(),
    _gamepadSelector("##active-gamepad", _gamepadManager.getNames()),
    _mouseSensitivity(_impulseProcessor.getMouseSensitivity()) {
	SDL_strlcpy(_urlBuffer, wsController.getUrl(), sizeof(_urlBuffer));
}

void ConfigSettingsPanel::show() {
	const ImVec2 childSize = ImGui::GetContentRegionAvail();
	if (ImGui::BeginChild("SettingsPanelChild",
	                      childSize,
	                      ImGuiChildFlags_None,
	                      ImGuiWindowFlags_NoSavedSettings)) {
		showVtsSettings();
		showGamepadSettings();
		showMouseMotionSettings();
		showMousePositionSettings();
	}
	showModals();
	ImGui::EndChild();
}

void ConfigSettingsPanel::setActiveGamepadIndex(const size_t activeIndex) {
	_gamepadSelector.setIndex(activeIndex);
}

}  // namespace gui
