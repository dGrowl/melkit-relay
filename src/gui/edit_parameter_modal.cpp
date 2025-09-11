#include <format>

#include "imgui/imgui.h"

#include "core/settings.hpp"
#include "gui/combo_box.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "gui/fonts.hpp"
#include "gui/utility.hpp"
#include "vts/parameter.hpp"
#include "vts/request.hpp"

namespace gui {

static constexpr const char* UNKNOWN = "UNKNOWN";

static constexpr const char* DEVICE_MOUSE    = "Mouse";
static constexpr const char* DEVICE_KEYBOARD = "Keyboard";
static constexpr const char* DEVICE_GAMEPAD  = "Controller";

static constexpr const char* MOUSE_EVENT_BUTTON        = "Button";
static constexpr const char* MOUSE_EVENT_MOVE_ABS      = "Move (Abs)";
static constexpr const char* MOUSE_EVENT_MOVE_REL      = "Move (Rel)";
static constexpr const char* KEY_EVENT_PRESS           = "Press";
static constexpr const char* GAMEPAD_EVENT_BUTTON      = "Button";
static constexpr const char* GAMEPAD_EVENT_TRIGGER     = "Trigger";
static constexpr const char* GAMEPAD_EVENT_STICK_LEFT  = "Motion (LStick)";
static constexpr const char* GAMEPAD_EVENT_STICK_RIGHT = "Motion (RStick)";

static const char* MOUSE_BUTTONS[] = {"Left",
                                      "Right",
                                      "Middle",
                                      "Fourth",
                                      "Fifth"};

static const char* GAMEPAD_BUTTONS[] = {"North",
                                        "South",
                                        "West",
                                        "East",
                                        "Shoulder (Left)",
                                        "Shoulder (Right)",
                                        "DPad (Up)",
                                        "DPad (Down)",
                                        "DPad (Left)",
                                        "DPad (Right)",
                                        "Stick (Left)",
                                        "Stick (Right)"};

static const char* GAMEPAD_TRIGGERS[] = {"Left", "Right"};

static const char* AXES[] = {"X", "Y"};

static constexpr unsigned BLEND_MODE_MAX         = 0;
static constexpr unsigned BLEND_MODE_BOUNDED_SUM = 1;

static std::vector<const char*> BLEND_MODES = {"Max", "Sum (Bound)"};

struct InputStrings {
	const char* device = UNKNOWN;
	const char* event  = UNKNOWN;
	const char* target = UNKNOWN;
};

InputStrings getInputStrings(const vts::InputId id) {
	InputStrings       strings;
	const vts::InputId event  = id & 0xFFFF;
	const vts::InputId target = id >> 16;
	switch (event) {
		case vts::InputEvent::KEY:
			strings.device = DEVICE_KEYBOARD;
			strings.event  = KEY_EVENT_PRESS;
			strings.target = getUioKeyName(target);
			break;
		case vts::InputEvent::MOUSE_BUTTON:
			strings.device = DEVICE_MOUSE;
			strings.event  = MOUSE_EVENT_BUTTON;
			strings.target = MOUSE_BUTTONS[target - 1];
			break;
		case vts::InputEvent::MOUSE_MOVE_ABS:
			strings.device = DEVICE_MOUSE;
			strings.event  = MOUSE_EVENT_MOVE_ABS;
			strings.target = AXES[target - 1];
			break;
		case vts::InputEvent::MOUSE_MOVE_REL:
			strings.device = DEVICE_MOUSE;
			strings.event  = MOUSE_EVENT_MOVE_REL;
			strings.target = AXES[target - 1];
			break;
		case vts::InputEvent::GAMEPAD_BUTTON:
			strings.device = DEVICE_GAMEPAD;
			strings.event  = GAMEPAD_EVENT_BUTTON;
			strings.target = GAMEPAD_BUTTONS[target - 1];
			break;
		case vts::InputEvent::GAMEPAD_TRIGGER:
			strings.device = DEVICE_GAMEPAD;
			strings.event  = GAMEPAD_EVENT_TRIGGER;
			strings.target = GAMEPAD_TRIGGERS[target - 1];
			break;
		case vts::InputEvent::GAMEPAD_STICK_LEFT:
			strings.device = DEVICE_GAMEPAD;
			strings.event  = GAMEPAD_EVENT_STICK_LEFT;
			strings.target = AXES[target - 1];
			break;
		case vts::InputEvent::GAMEPAD_STICK_RIGHT:
			strings.device = DEVICE_GAMEPAD;
			strings.event  = GAMEPAD_EVENT_STICK_RIGHT;
			strings.target = AXES[target - 1];
			break;
	}
	return strings;
}

void EditParameterModal::showAddInput() {
	if (ImGui::Button("Add", ImVec2(128.0f, 0.0f))) {
		ImGui::OpenPopup(AddInputModal::NAME);
	}

	_addInputModal.show();
}

void EditParameterModal::showInputs() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Input");
	}

	if (ImGui::BeginTable("Input Table", 6, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Device", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Target", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Invert", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Remove", ImGuiTableColumnFlags_WidthFixed);
		if (_editingParameter.hasInputs()) {
			ImGui::TableHeadersRow();
		}

		for (auto& [id, data] : _editingParameter.getInputs()) {
			ImGui::PushID(id);

			auto fields = getInputStrings(id);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(fields.device);

			ImGui::TableNextColumn();
			ImGui::Text(fields.event);

			ImGui::TableNextColumn();
			ImGui::Text(fields.target);

			ImGui::TableNextColumn();
			if (ImGui::Checkbox("##invert-value", &data.isInverted)) {
				_editingParameter.updateBounds();
			}

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(128.0f);
			ImGui::BeginDisabled();
			float value = data.getValue();
			ImGui::SliderFloat("##input-value",
			                   &value,
			                   data.getMin(),
			                   data.getMax(),
			                   "%.3f",
			                   ImGuiSliderFlags_NoInput);
			ImGui::EndDisabled();

			ImGui::TableNextColumn();
			const float rowHeight = ImGui::GetTextLineHeightWithSpacing();
			if (ImGui::Button("X", ImVec2(rowHeight, rowHeight))) {
				_inputIdToDelete = id;
			}

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

void EditParameterModal::showOutput() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Output");
	}

	if (ImGui::BeginTable("Output Table", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Blend Mode");
		ImGui::TableNextColumn();
		if (_blendModeSelector.show()) {
			updateBlendMode();
		}

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Minimum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::Text("%.2f", _editingParameter.getMin());

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Maximum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::Text("%.2f", _editingParameter.getMax());

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Current");
		ImGui::TableNextColumn();
		float outputValue = _editingParameter.getOutput();
		ImGui::SetNextItemWidth(128.0f);
		ImGui::BeginDisabled();
		ImGui::SliderFloat("##output-value",
		                   &outputValue,
		                   _editingParameter.getMin(),
		                   _editingParameter.getMax(),
		                   "%.2f",
		                   ImGuiSliderFlags_NoInput);
		ImGui::EndDisabled();

		ImGui::EndTable();
	}
}

void EditParameterModal::checkDeleteInput() {
	if (_inputIdToDelete == 0) {
		return;
	}
	_editingParameter.removeInput(_inputIdToDelete);
	_inputIdToDelete = 0;
}

void EditParameterModal::updateBlendMode() {
	switch (_blendModeSelector.getIndex()) {
		case BLEND_MODE_MAX:
			_editingParameter.setBlendMode(vts::BlendMode::MAX);
			break;
		case BLEND_MODE_BOUNDED_SUM:
			_editingParameter.setBlendMode(vts::BlendMode::BOUNDED_SUM);
			break;
	}
}

EditParameterModal::EditParameterModal(ws::IController& wsController,
                                       vts::Parameter&  editingParameter) :
    _wsController(wsController),
    _editingParameter(editingParameter),
    _addInputModal(editingParameter),
    _blendModeSelector("##blend-mode-selector", BLEND_MODES),
    _inputIdToDelete(0) {}

void EditParameterModal::refresh() {
	switch (_editingParameter.getBlendMode()) {
		case vts::BlendMode::MAX:
			_blendModeSelector.setIndex(BLEND_MODE_MAX);
			break;
		case vts::BlendMode::BOUNDED_SUM:
			_blendModeSelector.setIndex(BLEND_MODE_BOUNDED_SUM);
			break;
	}
}

void EditParameterModal::show() {
	if (ImGui::BeginPopupModal(
	        NAME,
	        nullptr,
	        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav)) {
		ImGui::Text(_editingParameter.getName().c_str());

		showInputs();
		showAddInput();
		showOutput();
		checkDeleteInput();

		if (ImGui::Button("Save", ImVec2(128.0f, 0.0f))) {
			SETTINGS.setParameter(_editingParameter);
			vts::createParameter(_wsController, _editingParameter);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Discard", ImVec2(128.0f, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

}  // namespace gui
