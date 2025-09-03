#include <format>

#include "imgui/imgui.h"

#include "core/settings.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "gui/utility.hpp"
#include "vts/request.hpp"

namespace gui {

static constexpr const char* UNKNOWN = "UNKNOWN";

static constexpr const char* DEVICE_MOUSE = "Mouse";
static constexpr const char* DEVICE_KEYBOARD = "Keyboard";

static constexpr const char* KEY_EVENT_PRESS = "Press";
static constexpr const char* MOUSE_EVENT_BUTTON = "Button";
static constexpr const char* MOUSE_EVENT_MOVE_ABS = "Move (Abs)";
static constexpr const char* MOUSE_EVENT_MOVE_REL = "Move (Rel)";

static const char* MOUSE_BUTTONS[] = {"Left",
                                      "Right",
                                      "Middle",
                                      "Fourth",
                                      "Fifth"};

static const char* AXES[]{"X", "Y"};

struct InputStrings {
	const char* device = UNKNOWN;
	const char* event = UNKNOWN;
	const char* target = UNKNOWN;
};

InputStrings getInputStrings(const vts::InputId id) {
	InputStrings strings;
	const vts::InputId event = id & 0xFFFF;
	const vts::InputId target = id >> 16;
	switch (event) {
		case vts::InputEvent::MOUSE_BUTTON:
			strings.device = DEVICE_MOUSE;
			strings.event = MOUSE_EVENT_BUTTON;
			strings.target = MOUSE_BUTTONS[target - 1];
			break;
		case vts::InputEvent::MOUSE_MOVE_ABS:
			strings.device = DEVICE_MOUSE;
			strings.event = MOUSE_EVENT_MOVE_ABS;
			strings.target = AXES[target - 1];
			break;
		case vts::InputEvent::MOUSE_MOVE_REL:
			strings.device = DEVICE_MOUSE;
			strings.event = MOUSE_EVENT_MOVE_REL;
			strings.target = AXES[target - 1];
			break;
		case vts::InputEvent::KEY:
			strings.device = DEVICE_KEYBOARD;
			strings.event = KEY_EVENT_PRESS;
			strings.target = getUioKeyName(target);
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
	ImGui::SeparatorText("Input");

	if (ImGui::BeginTable("Input Table", 5, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Device", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Target", ImGuiTableColumnFlags_WidthFixed);
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
			ImGui::SetNextItemWidth(128.0f);
			ImGui::BeginDisabled();
			ImGui::SliderFloat("##input-value",
			                   &data.value,
			                   data.min,
			                   data.max,
			                   "%.3f",
			                   ImGuiSliderFlags_NoInput);
			ImGui::EndDisabled();
			ImGui::TableNextColumn();
			if (ImGui::Button("x", ImVec2(16.0f, 0.0f))) {
				_inputIdToDelete = id;
			}

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

void EditParameterModal::showOutput() {
	ImGui::SeparatorText("Output");

	if (ImGui::BeginTable("Output Table", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Default");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##default",
		                   ImGuiDataType_Float,
		                   &_editingParameter.defaultValue,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Minimum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##minimum",
		                   ImGuiDataType_Float,
		                   &_editingParameter.min,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Maximum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##maximum",
		                   ImGuiDataType_Float,
		                   &_editingParameter.max,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Current");
		ImGui::TableNextColumn();
		float outputValue = _editingParameter.getOutput();
		ImGui::SetNextItemWidth(128.0f);
		ImGui::BeginDisabled();
		ImGui::SliderFloat("##output-value",
		                   &outputValue,
		                   _editingParameter.min,
		                   _editingParameter.max,
		                   "%.3f",
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

EditParameterModal::EditParameterModal(ws::IController& wsController,
                                       vts::Parameter& editingParameter) :
    _wsController(wsController),
    _editingParameter(editingParameter),
    _addInputModal(editingParameter),
    _inputIdToDelete(0) {}

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
