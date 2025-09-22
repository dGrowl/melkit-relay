#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "gui/add_input_modal.hpp"
#include "gui/utility.hpp"

namespace gui {

static constexpr unsigned DEVICE_MOUSE    = 0;
static constexpr unsigned DEVICE_KEYBOARD = 1;
static constexpr unsigned DEVICE_GAMEPAD  = 2;

static std::vector<const char*> DEVICES{"Mouse", "Keyboard", "Controller"};

static constexpr unsigned MOUSE_EVENT_BUTTON        = 0;
static constexpr unsigned MOUSE_EVENT_MOVE_ABSOLUTE = 1;
static constexpr unsigned MOUSE_EVENT_MOVE_RELATIVE = 2;

static std::vector<const char*> MOUSE_EVENTS{"Button", "Position", "Motion"};

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

static constexpr unsigned GAMEPAD_EVENT_BUTTON      = 0;
static constexpr unsigned GAMEPAD_EVENT_LEFT_STICK  = 1;
static constexpr unsigned GAMEPAD_EVENT_RIGHT_STICK = 2;
static constexpr unsigned GAMEPAD_EVENT_TRIGGER     = 3;

static std::vector<const char*> GAMEPAD_EVENTS{"Button",
                                               "Stick (Left)",
                                               "Stick (Right)",
                                               "Trigger"};

static constexpr unsigned GAMEPAD_BUTTON_NORTH          = 0;
static constexpr unsigned GAMEPAD_BUTTON_SOUTH          = 1;
static constexpr unsigned GAMEPAD_BUTTON_WEST           = 2;
static constexpr unsigned GAMEPAD_BUTTON_EAST           = 3;
static constexpr unsigned GAMEPAD_BUTTON_LEFT_SHOULDER  = 4;
static constexpr unsigned GAMEPAD_BUTTON_RIGHT_SHOULDER = 5;
static constexpr unsigned GAMEPAD_BUTTON_DPAD_UP        = 6;
static constexpr unsigned GAMEPAD_BUTTON_DPAD_DOWN      = 7;
static constexpr unsigned GAMEPAD_BUTTON_DPAD_LEFT      = 8;
static constexpr unsigned GAMEPAD_BUTTON_DPAD_RIGHT     = 9;

static std::vector<const char*> GAMEPAD_BUTTONS{"North",
                                                "South",
                                                "West",
                                                "East",
                                                "Shoulder (Left)",
                                                "Shoulder (Right)",
                                                "DPad (Up)",
                                                "DPad (Down)",
                                                "DPad (Left)",
                                                "DPad (Right)"};

static constexpr unsigned GAMEPAD_STICK_ACTION_X     = 0;
static constexpr unsigned GAMEPAD_STICK_ACTION_Y     = 1;
static constexpr unsigned GAMEPAD_STICK_ACTION_PRESS = 2;

static std::vector<const char*> GAMEPAD_STICK_ACTIONS{"X (Left-Right)",
                                                      "Y (Up-Down)",
                                                      "Press"};

static constexpr unsigned GAMEPAD_SIDE_LEFT  = 0;
static constexpr unsigned GAMEPAD_SIDE_RIGHT = 1;

static std::vector<const char*> GAMEPAD_SIDES{"Left", "Right"};

vts::TargetTag AddInputModal::getMouseAxisTag() const {
	switch (_mouseAxisSelector.getIndex()) {
		case MOUSE_AXIS_X:
			return vts::Axis::X;
		case MOUSE_AXIS_Y:
			return vts::Axis::Y;
	}
	return 0;
}

vts::TargetTag AddInputModal::getMouseButtonTag() const {
	switch (_mouseButtonSelector.getIndex()) {
		case MOUSE_BUTTON_LEFT:
			return vts::MouseButton::LEFT;
		case MOUSE_BUTTON_RIGHT:
			return vts::MouseButton::RIGHT;
		case MOUSE_BUTTON_MIDDLE:
			return vts::MouseButton::MIDDLE;
		case MOUSE_BUTTON_FOURTH:
			return vts::MouseButton::FOURTH;
		case MOUSE_BUTTON_FIFTH:
			return vts::MouseButton::FIFTH;
	}
	return 0;
}

vts::TargetTag AddInputModal::getGamepadButtonTag() const {
	switch (_gamepadButtonSelector.getIndex()) {
		case GAMEPAD_BUTTON_NORTH:
			return vts::GamepadButton::NORTH;
		case GAMEPAD_BUTTON_SOUTH:
			return vts::GamepadButton::SOUTH;
		case GAMEPAD_BUTTON_WEST:
			return vts::GamepadButton::WEST;
		case GAMEPAD_BUTTON_EAST:
			return vts::GamepadButton::EAST;
		case GAMEPAD_BUTTON_LEFT_SHOULDER:
			return vts::GamepadButton::LEFT_SHOULDER;
		case GAMEPAD_BUTTON_RIGHT_SHOULDER:
			return vts::GamepadButton::RIGHT_SHOULDER;
		case GAMEPAD_BUTTON_DPAD_UP:
			return vts::GamepadButton::DPAD_UP;
		case GAMEPAD_BUTTON_DPAD_DOWN:
			return vts::GamepadButton::DPAD_DOWN;
		case GAMEPAD_BUTTON_DPAD_LEFT:
			return vts::GamepadButton::DPAD_LEFT;
		case GAMEPAD_BUTTON_DPAD_RIGHT:
			return vts::GamepadButton::DPAD_RIGHT;
	}
	return 0;
}

vts::TargetTag AddInputModal::getGamepadStickActionTag(
    const bool isLeftStick) const {
	vts::TargetTag id = 0;
	switch (_gamepadStickActionSelector.getIndex()) {
		case GAMEPAD_STICK_ACTION_X:
			id |= isLeftStick ? vts::InputEvent::GAMEPAD_STICK_LEFT
			                  : vts::InputEvent::GAMEPAD_STICK_RIGHT;
			id |= vts::Axis::X;
			break;
		case GAMEPAD_STICK_ACTION_Y:
			id |= isLeftStick ? vts::InputEvent::GAMEPAD_STICK_LEFT
			                  : vts::InputEvent::GAMEPAD_STICK_RIGHT;
			id |= vts::Axis::Y;
			break;
		case GAMEPAD_STICK_ACTION_PRESS:
			id |= vts::InputEvent::GAMEPAD_BUTTON;
			id |= isLeftStick ? vts::GamepadButton::LEFT_STICK
			                  : vts::GamepadButton::RIGHT_STICK;
			break;
	}
	return id;
}

vts::TargetTag AddInputModal::getGamepadTriggerTag() const {
	switch (_gamepadTriggerSelector.getIndex()) {
		case GAMEPAD_SIDE_LEFT:
			return vts::Side::LEFT;
		case GAMEPAD_SIDE_RIGHT:
			return vts::Side::RIGHT;
	}
	return 0;
}

vts::InputId AddInputModal::buildInputId() const {
	vts::InputId id     = 0;
	const auto   device = _deviceSelector.getIndex();
	if (device == DEVICE_MOUSE) {
		const auto event = _mouseEventSelector.getIndex();
		switch (event) {
			case MOUSE_EVENT_BUTTON:
				id |= vts::InputEvent::MOUSE_BUTTON;
				id |= getMouseButtonTag();
				break;
			case MOUSE_EVENT_MOVE_ABSOLUTE:
				id |= vts::InputEvent::MOUSE_MOVE_ABS;
				id |= getMouseAxisTag();
				break;
			case MOUSE_EVENT_MOVE_RELATIVE:
				id |= vts::InputEvent::MOUSE_MOVE_REL;
				id |= getMouseAxisTag();
				break;
		}
	}
	else if (device == DEVICE_KEYBOARD) {
		id |= vts::InputEvent::KEY;
		if (_selectedKey != ImGuiKey_None) {
			id |= gui::convertImGuiToUioKey(_selectedKey) << 16;
		}
	}
	else if (device == DEVICE_GAMEPAD) {
		const auto event = _gamepadEventSelector.getIndex();
		switch (event) {
			case GAMEPAD_EVENT_BUTTON:
				id |= vts::InputEvent::GAMEPAD_BUTTON;
				id |= getGamepadButtonTag();
				break;
			case GAMEPAD_EVENT_LEFT_STICK:
				id |= getGamepadStickActionTag(true);
				break;
			case GAMEPAD_EVENT_RIGHT_STICK:
				id |= getGamepadStickActionTag(false);
				break;
			case GAMEPAD_EVENT_TRIGGER:
				id |= vts::InputEvent::GAMEPAD_TRIGGER;
				id |= getGamepadTriggerTag();
				break;
		}
	};
	return id;
}

void AddInputModal::showCloseButtons() {
	if (ImGui::Button("Add", ImVec2(128.0f, 0.0f))) {
		vts::InputId id = buildInputId();
		if (id != vts::InputEvent::KEY) {
			_editingParameter.addInput(id);
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

void AddInputModal::showGamepadButtonSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Button");
	ImGui::TableNextColumn();
	_gamepadButtonSelector.show();
}

void AddInputModal::showGamepadControls() {
	showGamepadEventSelector();
	switch (_gamepadEventSelector.getIndex()) {
		case GAMEPAD_EVENT_BUTTON:
			showGamepadButtonSelector();
			break;
		case GAMEPAD_EVENT_LEFT_STICK:
		case GAMEPAD_EVENT_RIGHT_STICK:
			showGamepadStickActionSelector();
			break;
		case GAMEPAD_EVENT_TRIGGER:
			showGamepadTriggerSelector();
			break;
	}
}

void AddInputModal::showGamepadEventSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Event");
	ImGui::TableNextColumn();
	_gamepadEventSelector.show();
}

void AddInputModal::showGamepadStickActionSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Action");
	ImGui::TableNextColumn();
	_gamepadStickActionSelector.show();
}

void AddInputModal::showGamepadTriggerSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Side");
	ImGui::TableNextColumn();
	_gamepadTriggerSelector.show();
}

AddInputModal::AddInputModal(vts::Parameter& editingParameter) :
    _selectedKeyName(),
    _selectedKey(ImGuiKey_None),
    _editingParameter(editingParameter),
    _deviceSelector("##device-selector", DEVICES),
    _mouseAxisSelector("##mouse-axis-selector", AXES),
    _mouseButtonSelector("##mouse-button-selector", MOUSE_BUTTONS),
    _mouseEventSelector("##mouse-event-selector", MOUSE_EVENTS),
    _gamepadButtonSelector("##gamepad-button-selector", GAMEPAD_BUTTONS),
    _gamepadEventSelector("##gamepad-event-selector", GAMEPAD_EVENTS),
    _gamepadStickActionSelector("##gamepad-stick-action-selector",
                                GAMEPAD_STICK_ACTIONS),
    _gamepadTriggerSelector("##gamepad-trigger-selector", GAMEPAD_SIDES) {}

void AddInputModal::show() {
	centerNextWindow();
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
				case DEVICE_GAMEPAD:
					showGamepadControls();
					break;
			}

			ImGui::EndTable();
		}

		showCloseButtons();
		ImGui::EndPopup();
	}
}

}  // namespace gui
