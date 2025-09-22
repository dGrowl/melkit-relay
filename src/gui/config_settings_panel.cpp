#include <cmath>
#include <format>

#include "imgui/imgui.h"

#include "gui/config_settings_panel.hpp"
#include "gui/fonts.hpp"

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
		ImGui::SetNextItemWidth(-1.0f);
		if (_gamepadSelector.show()) {
			_gamepadManager.setActive(_gamepadSelector.getIndex());
		}

		ImGui::EndTable();
	}

	if (ImGui::Button("Refresh", ImVec2(-1.0f, 0.0f))) {
		_gamepadManager.refreshDevices();
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
		ImGui::SetNextItemWidth(-1.0f);
		if (ImGui::SliderInt("##mouse-sensitivity-slider",
		                     &_mouseSensitivity,
		                     1,
		                     100)) {
			_parameterManager.setMouseSensitivity(_mouseSensitivity);
		}
		ImGui::SetItemTooltip(
		    "Makes mouse motion inputs more or less intense.\n\nYou can Ctrl + Click "
		    "to type in a value.");

		ImGui::EndTable();
	}

	ImGui::Spacing();
}

void ConfigSettingsPanel::showMousePositionSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Mouse (Position)");
	}

	ImGui::Text("Region");

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImVec2 cursorPos      = ImGui::GetCursorScreenPos();
	float  availableWidth = ImGui::GetContentRegionAvail().x;
	float  desiredHeight  = 64.0f;

	ImVec2 rectMin = cursorPos;
	ImVec2 rectMax(cursorPos.x + availableWidth - 1.0f,
	               cursorPos.y + desiredHeight);

	ImU32 fillColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
	ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

	drawList->AddRectFilled(rectMin, rectMax, fillColor);

	const auto& mouseBounds = _parameterManager.getMouseBounds();
	auto topLeftString = std::format("({}, {})", mouseBounds.left, mouseBounds.top);
	auto bottomRightString = std::format("({}, {})", mouseBounds.right, mouseBounds.bottom);
	const char* topLeftText = topLeftString.c_str();
	const char* bottomRightText = bottomRightString.c_str();

	ImVec2 topLeftTextPos = rectMin;
	topLeftTextPos.x += 3.0f;
	topLeftTextPos.y += 2.0f;

	ImVec2 bottomRightTextSize = ImGui::CalcTextSize(bottomRightText);
	ImVec2 bottomRightTextPos  = rectMax;
	bottomRightTextPos.x -= bottomRightTextSize.x + 3.0f;
	bottomRightTextPos.y -= bottomRightTextSize.y + 2.0f;

	drawList->AddText(topLeftTextPos, textColor, topLeftText);
	drawList->AddText(bottomRightTextPos, textColor, bottomRightText);

	ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, rectMax.y));
	ImGui::Spacing();

	if (ImGui::Button("Adjust", ImVec2(-1.0f, 0.0f))) {
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
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
		if (ImGui::InputText("##vts-url", _urlBuffer, IM_ARRAYSIZE(_urlBuffer))) {
			_wsController.setUrl(_urlBuffer);
		}
		ImGui::PopItemFlag();

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

	ImGui::Spacing();
}

void ConfigSettingsPanel::showModals() {
	_setMouseBoundsModal.show();
}

ConfigSettingsPanel::ConfigSettingsPanel(
    pad::Manager&          gamepadManager,
    vts::ParameterManager& parameterManager,
    ws::IController&       wsController) :
    _gamepadManager(gamepadManager),
    _parameterManager(parameterManager),
    _wsController(wsController),
    _setMouseBoundsModal(parameterManager),
    _urlBuffer(),
    _gamepadSelector("##active-gamepad", _gamepadManager.getNames()),
    _mouseSensitivity(_parameterManager.getMouseSensitivity()) {
	SDL_strlcpy(_urlBuffer, wsController.getUrl(), sizeof(_urlBuffer));
}

void ConfigSettingsPanel::show() {
	ImVec2 childSize = ImGui::GetContentRegionAvail();
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
