#include "gui/add_impulse_modal.hpp"

#include <string>
#include <vector>

#include "imgui/imgui.h"

#include "gui/utility.hpp"
#include "impulse/code.hpp"

namespace gui {

static constexpr unsigned DEVICE_MOUSE    = 0;
static constexpr unsigned DEVICE_KEYBOARD = 1;
static constexpr unsigned DEVICE_GAMEPAD  = 2;

static std::vector<const char*> DEVICES{"Mouse", "Keyboard", "Controller"};

static constexpr unsigned MOUSE_EVENT_BUTTON        = 0;
static constexpr unsigned MOUSE_EVENT_WHEEL         = 1;
static constexpr unsigned MOUSE_EVENT_MOVE_ABSOLUTE = 2;
static constexpr unsigned MOUSE_EVENT_MOVE_RELATIVE = 3;

static std::vector<const char*> MOUSE_EVENTS{"Button",
                                             "Wheel",
                                             "Position",
                                             "Motion"};

static constexpr unsigned MOUSE_BUTTON_LEFT   = 0;
static constexpr unsigned MOUSE_BUTTON_RIGHT  = 1;
static constexpr unsigned MOUSE_BUTTON_MIDDLE = 2;

static std::vector<const char*> MOUSE_BUTTONS{"Left", "Right", "Middle"};

static constexpr unsigned MOUSE_WHEEL_UP   = 0;
static constexpr unsigned MOUSE_WHEEL_DOWN = 1;

static std::vector<const char*> MOUSE_WHEEL_DIRECTIONS{"Up", "Down"};

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

imp::Axis::T AddImpulseModal::getMouseAxisTag() const {
	switch (_mouseAxisSelector.getIndex()) {
		case MOUSE_AXIS_X:
			return imp::Axis::X;
		case MOUSE_AXIS_Y:
			return imp::Axis::Y;
	}
	return 0;
}

imp::MouseButton::T AddImpulseModal::getMouseButtonTag() const {
	switch (_mouseButtonSelector.getIndex()) {
		case MOUSE_BUTTON_LEFT:
			return imp::MouseButton::LEFT;
		case MOUSE_BUTTON_RIGHT:
			return imp::MouseButton::RIGHT;
		case MOUSE_BUTTON_MIDDLE:
			return imp::MouseButton::MIDDLE;
	}
	return 0;
}

imp::MouseWheel::T AddImpulseModal::getMouseWheelTag() const {
	switch (_mouseWheelSelector.getIndex()) {
		case MOUSE_WHEEL_UP:
			return imp::MouseWheel::UP;
		case MOUSE_WHEEL_DOWN:
			return imp::MouseWheel::DOWN;
	}
	return 0;
}

imp::Code AddImpulseModal::getGamepadStickActionCode(
    const bool isLeftStick) const {
	imp::Code code = 0;
	switch (_gamepadStickActionSelector.getIndex()) {
		case GAMEPAD_STICK_ACTION_X:
			code |= isLeftStick ? imp::EventTag::GAMEPAD_STICK_LEFT
			                    : imp::EventTag::GAMEPAD_STICK_RIGHT;
			code |= imp::Axis::X;
			break;
		case GAMEPAD_STICK_ACTION_Y:
			code |= isLeftStick ? imp::EventTag::GAMEPAD_STICK_LEFT
			                    : imp::EventTag::GAMEPAD_STICK_RIGHT;
			code |= imp::Axis::Y;
			break;
		case GAMEPAD_STICK_ACTION_PRESS:
			code |= imp::EventTag::GAMEPAD_BUTTON;
			code |= isLeftStick ? imp::GamepadButton::LEFT_STICK
			                    : imp::GamepadButton::RIGHT_STICK;
			break;
	}
	return code;
}

imp::GamepadButton::T AddImpulseModal::getGamepadButtonTag() const {
	switch (_gamepadButtonSelector.getIndex()) {
		case GAMEPAD_BUTTON_NORTH:
			return imp::GamepadButton::NORTH;
		case GAMEPAD_BUTTON_SOUTH:
			return imp::GamepadButton::SOUTH;
		case GAMEPAD_BUTTON_WEST:
			return imp::GamepadButton::WEST;
		case GAMEPAD_BUTTON_EAST:
			return imp::GamepadButton::EAST;
		case GAMEPAD_BUTTON_LEFT_SHOULDER:
			return imp::GamepadButton::LEFT_SHOULDER;
		case GAMEPAD_BUTTON_RIGHT_SHOULDER:
			return imp::GamepadButton::RIGHT_SHOULDER;
		case GAMEPAD_BUTTON_DPAD_UP:
			return imp::GamepadButton::DPAD_UP;
		case GAMEPAD_BUTTON_DPAD_DOWN:
			return imp::GamepadButton::DPAD_DOWN;
		case GAMEPAD_BUTTON_DPAD_LEFT:
			return imp::GamepadButton::DPAD_LEFT;
		case GAMEPAD_BUTTON_DPAD_RIGHT:
			return imp::GamepadButton::DPAD_RIGHT;
	}
	return 0;
}

imp::TargetTag AddImpulseModal::getGamepadTriggerTag() const {
	switch (_gamepadTriggerSelector.getIndex()) {
		case GAMEPAD_SIDE_LEFT:
			return imp::Side::LEFT;
		case GAMEPAD_SIDE_RIGHT:
			return imp::Side::RIGHT;
	}
	return 0;
}

imp::Code AddImpulseModal::buildImpulseCode() const {
	imp::Code  code   = 0;
	const auto device = _deviceSelector.getIndex();
	if (device == DEVICE_MOUSE) {
		const auto event = _mouseEventSelector.getIndex();
		switch (event) {
			case MOUSE_EVENT_BUTTON:
				code |= imp::EventTag::MOUSE_BUTTON;
				code |= getMouseButtonTag();
				break;
			case MOUSE_EVENT_WHEEL:
				code |= imp::EventTag::MOUSE_WHEEL;
				code |= getMouseWheelTag();
				break;
			case MOUSE_EVENT_MOVE_ABSOLUTE:
				code |= imp::EventTag::MOUSE_MOVE_ABS;
				code |= getMouseAxisTag();
				break;
			case MOUSE_EVENT_MOVE_RELATIVE:
				code |= imp::EventTag::MOUSE_MOVE_REL;
				code |= getMouseAxisTag();
				break;
		}
	}
	else if (device == DEVICE_KEYBOARD) {
		code |= imp::EventTag::KEY;
		if (_selectedKey != ImGuiKey_None) {
			code |= gui::convertImGuiToUioKey(_selectedKey) << 16;
		}
	}
	else if (device == DEVICE_GAMEPAD) {
		const auto event = _gamepadEventSelector.getIndex();
		switch (event) {
			case GAMEPAD_EVENT_BUTTON:
				code |= imp::EventTag::GAMEPAD_BUTTON;
				code |= getGamepadButtonTag();
				break;
			case GAMEPAD_EVENT_LEFT_STICK:
				code = getGamepadStickActionCode(true);
				break;
			case GAMEPAD_EVENT_RIGHT_STICK:
				code = getGamepadStickActionCode(false);
				break;
			case GAMEPAD_EVENT_TRIGGER:
				code |= imp::EventTag::GAMEPAD_TRIGGER;
				code |= getGamepadTriggerTag();
				break;
		}
	};
	return code;
}

void AddImpulseModal::showCloseButtons() {
	if (ImGui::Button("Add", ImVec2(128.0F, 0.0F))) {
		imp::Code code = buildImpulseCode();
		if (code != imp::EventTag::KEY) {
			_editingParameter.addImpulse(code);
			ImGui::CloseCurrentPopup();
		}
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(128.0F, 0.0F))) {
		ImGui::CloseCurrentPopup();
	}
}

void AddImpulseModal::showDeviceSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Device");
	ImGui::TableNextColumn();
	_deviceSelector.show();
}

void AddImpulseModal::showMouseEventSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Event");
	ImGui::TableNextColumn();
	_mouseEventSelector.show();
}

void AddImpulseModal::showMouseButtonSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Button");
	ImGui::TableNextColumn();
	_mouseButtonSelector.show();
}

void AddImpulseModal::showMouseAxisSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Axis");
	ImGui::TableNextColumn();
	_mouseAxisSelector.show();
}

void AddImpulseModal::showMouseWheelSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Direction");
	ImGui::TableNextColumn();
	_mouseWheelSelector.show();
}

void AddImpulseModal::showMouseControls() {
	showMouseEventSelector();
	switch (_mouseEventSelector.getIndex()) {
		case MOUSE_EVENT_BUTTON:
			showMouseButtonSelector();
			break;
		case MOUSE_EVENT_WHEEL:
			showMouseWheelSelector();
			break;
		case MOUSE_EVENT_MOVE_ABSOLUTE:
		case MOUSE_EVENT_MOVE_RELATIVE:
			showMouseAxisSelector();
			break;
	}
}

void AddImpulseModal::showKeyboardControls() {
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

void AddImpulseModal::showGamepadButtonSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Button");
	ImGui::TableNextColumn();
	_gamepadButtonSelector.show();
}

void AddImpulseModal::showGamepadControls() {
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

void AddImpulseModal::showGamepadEventSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Event");
	ImGui::TableNextColumn();
	_gamepadEventSelector.show();
}

void AddImpulseModal::showGamepadStickActionSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Action");
	ImGui::TableNextColumn();
	_gamepadStickActionSelector.show();
}

void AddImpulseModal::showGamepadTriggerSelector() {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	ImGui::Text("Side");
	ImGui::TableNextColumn();
	_gamepadTriggerSelector.show();
}

AddImpulseModal::AddImpulseModal(vts::Parameter& editingParameter) :
    _selectedKeyName(),
    _selectedKey(ImGuiKey_None),
    _editingParameter(editingParameter),
    _deviceSelector("##device-selector", DEVICES),
    _mouseAxisSelector("##mouse-axis-selector", AXES),
    _mouseButtonSelector("##mouse-button-selector", MOUSE_BUTTONS),
    _mouseEventSelector("##mouse-event-selector", MOUSE_EVENTS),
    _mouseWheelSelector("##mouse-wheel-selector", MOUSE_WHEEL_DIRECTIONS),
    _gamepadButtonSelector("##gamepad-button-selector", GAMEPAD_BUTTONS),
    _gamepadEventSelector("##gamepad-event-selector", GAMEPAD_EVENTS),
    _gamepadStickActionSelector("##gamepad-stick-action-selector",
                                GAMEPAD_STICK_ACTIONS),
    _gamepadTriggerSelector("##gamepad-trigger-selector", GAMEPAD_SIDES) {}

void AddImpulseModal::show() {
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
