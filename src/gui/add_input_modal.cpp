#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "gui/add_input_modal.hpp"
#include "gui/utility.hpp"

namespace gui {

static constexpr unsigned DEVICE_MOUSE    = 0;
static constexpr unsigned DEVICE_KEYBOARD = 1;

static std::vector<const char*> DEVICES{"Mouse", "Keyboard"};

static constexpr unsigned MOUSE_EVENT_BUTTON        = 0;
static constexpr unsigned MOUSE_EVENT_MOVE_ABSOLUTE = 1;
static constexpr unsigned MOUSE_EVENT_MOVE_RELATIVE = 2;

static std::vector<const char*> MOUSE_EVENTS{"Button",
                                             "Move (Absolute)",
                                             "Move (Relative)"};

static constexpr unsigned MOUSE_BUTTON_LEFT   = 0;
static constexpr unsigned MOUSE_BUTTON_RIGHT  = 1;
static constexpr unsigned MOUSE_BUTTON_MIDDLE = 2;
static constexpr unsigned MOUSE_BUTTON_FOURTH = 3;
static constexpr unsigned MOUSE_BUTTON_FIFTH  = 4;

static std::vector<const char*> MOUSE_BUTTONS{"Left",
                                              "Right",
                                              "Middle",
                                              "Fourth",
                                              "Fifth"};

static constexpr unsigned MOUSE_AXIS_X = 0;
static constexpr unsigned MOUSE_AXIS_Y = 1;

static std::vector<const char*> AXES{"X (Left-Right)", "Y (Up-Down)"};

vts::InputId AddInputModal::getMouseAxisId() const {
	switch (_mouseAxisSelector.getIndex()) {
		case MOUSE_AXIS_X:
			return vts::Axis::X;
		case MOUSE_AXIS_Y:
			return vts::Axis::Y;
	}
	return 0;
}

vts::InputId AddInputModal::getMouseButtonId() const {
	switch (_mouseButtonSelector.getIndex()) {
		case MOUSE_BUTTON_LEFT:
			return vts::Button::LEFT;
		case MOUSE_BUTTON_RIGHT:
			return vts::Button::RIGHT;
		case MOUSE_BUTTON_MIDDLE:
			return vts::Button::MIDDLE;
		case MOUSE_BUTTON_FOURTH:
			return vts::Button::FOURTH;
		case MOUSE_BUTTON_FIFTH:
			return vts::Button::FIFTH;
	}
	return 0;
}

vts::InputData AddInputModal::buildInputData() const {
	vts::InputId id     = 0;
	const auto   device = _deviceSelector.getIndex();
	if (device == DEVICE_MOUSE) {
		const auto event = _mouseEventSelector.getIndex();
		switch (event) {
			case MOUSE_EVENT_BUTTON:
				id |= vts::InputEvent::MOUSE_BUTTON;
				id |= getMouseButtonId();
				break;
			case MOUSE_EVENT_MOVE_ABSOLUTE:
				id |= vts::InputEvent::MOUSE_MOVE_ABS;
				id |= getMouseAxisId();
				break;
			case MOUSE_EVENT_MOVE_RELATIVE:
				id |= vts::InputEvent::MOUSE_MOVE_REL;
				id |= getMouseAxisId();
				break;
		}
	}
	else if (device == DEVICE_KEYBOARD) {
		id |= vts::InputEvent::KEY;
		if (_selectedKey != ImGuiKey_None) {
			id |= gui::convertImGuiToUioKey(_selectedKey) << 16;
		}
	};
	return vts::InputData(id);
}

void AddInputModal::showCloseButtons() {
	if (ImGui::Button("Add", ImVec2(128.0f, 0.0f))) {
		vts::InputData data = buildInputData();
		if (data.getId() != vts::InputEvent::KEY) {
			_editingParameter.addInput(data);
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(128.0f, 0.0f))) {
		ImGui::CloseCurrentPopup();
	}
}

void AddInputModal::showDeviceSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Device");
	ImGui::TableNextColumn();
	_deviceSelector.show();
}

void AddInputModal::showMouseEventSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Event");
	ImGui::TableNextColumn();
	_mouseEventSelector.show();
}

void AddInputModal::showMouseButtonSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Button");
	ImGui::TableNextColumn();
	_mouseButtonSelector.show();
}

void AddInputModal::showMouseAxisSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Axis");
	ImGui::TableNextColumn();
	_mouseAxisSelector.show();
}

void AddInputModal::showMouseControls() {
	showMouseEventSelector();
	switch (_mouseEventSelector.getIndex()) {
		case MOUSE_EVENT_BUTTON:
			showMouseButtonSelector();
			break;
		case MOUSE_EVENT_MOVE_ABSOLUTE:
		case MOUSE_EVENT_MOVE_RELATIVE:
			showMouseAxisSelector();
			break;
	}
}

void AddInputModal::showKeyboardControls() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Event");
	ImGui::TableNextColumn();
	ImGui::Text("Held");

	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Key");
	ImGui::TableNextColumn();

	for (const ImGuiKey key : gui::validImGuiKeys()) {
		if (ImGui::IsKeyPressed(key)) {
			_selectedKey     = key;
			_selectedKeyName = ImGui::GetKeyName(key);
			break;
		}
	}

	ImGui::InputText("##key-listener",
	                 _selectedKeyName.data(),
	                 _selectedKeyName.length() + 1,
	                 ImGuiInputTextFlags_ReadOnly);
}

AddInputModal::AddInputModal(vts::Parameter& editingParameter) :
    _selectedKeyName(),
    _selectedKey(ImGuiKey_None),
    _editingParameter(editingParameter),
    _deviceSelector("##device-selector", DEVICES),
    _mouseAxisSelector("##mouse-axis-selector", AXES),
    _mouseButtonSelector("##mouse-button-selector", MOUSE_BUTTONS),
    _mouseEventSelector("##mouse-event-selector", MOUSE_EVENTS) {}

void AddInputModal::show() {
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoNav)) {
		if (ImGui::BeginTable("Input Table", 2, ImGuiTableFlags_SizingFixedFit)) {
			ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

			showDeviceSelector();
			switch (_deviceSelector.getIndex()) {
				case DEVICE_MOUSE:
					showMouseControls();
					break;
				case DEVICE_KEYBOARD:
					showKeyboardControls();
					break;
			}

			ImGui::EndTable();
		}

		showCloseButtons();
		ImGui::EndPopup();
	}
}

}  // namespace gui
