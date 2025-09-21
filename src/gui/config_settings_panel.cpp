#include <cmath>

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

void ConfigSettingsPanel::showMouseSettings() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Mouse");
	}

	if (ImGui::BeginTable("Mouse Settings", 2, ImGuiTableFlags_SizingFixedFit)) {
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
}

void ConfigSettingsPanel::showVtsSettings() {
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
}

ConfigSettingsPanel::ConfigSettingsPanel(
    pad::Manager&          gamepadManager,
    vts::ParameterManager& parameterManager,
    ws::IController&       wsController) :
    _gamepadManager(gamepadManager),
    _parameterManager(parameterManager),
    _wsController(wsController),
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
		showMouseSettings();
	}
	ImGui::EndChild();
}

void ConfigSettingsPanel::setActiveGamepadIndex(const size_t activeIndex) {
	_gamepadSelector.setIndex(activeIndex);
}

}  // namespace gui
