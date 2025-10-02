#include "gui/parameter_template_modal.hpp"

#include <array>

#include "imgui/imgui.h"
#include "libuiohook/uiohook.h"

#include "core/settings.hpp"
#include "gui/utility.hpp"
#include "vts/parameter.hpp"
#include "vts/request.hpp"

static constexpr size_t TEMPLATE_CONTROLLER = 0;

static std::vector<const char*> TEMPLATES{"Controller"};

const std::array<imp::InputId, 2> PRESS_LEFT_KEY_INPUTS = {
    VC_CONTROL_L << 16 | imp::InputEvent::KEY,
    VC_SHIFT_L << 16 | imp::InputEvent::KEY};
const std::array<imp::InputId, 4> PRESS_RIGHT_KEY_INPUTS = {
    VC_R << 16 | imp::InputEvent::KEY,
    VC_F << 16 | imp::InputEvent::KEY,
    VC_G << 16 | imp::InputEvent::KEY,
    VC_SPACE << 16 | imp::InputEvent::KEY,
};
const std::array<imp::InputId, 1> PRESS_LEFT_GAMEPAD_INPUTS = {
    imp::GamepadButton::LEFT_STICK | imp::InputEvent::GAMEPAD_BUTTON};
const std::array<imp::InputId, 5> PRESS_RIGHT_GAMEPAD_INPUTS = {
    imp::GamepadButton::NORTH | imp::InputEvent::GAMEPAD_BUTTON,
    imp::GamepadButton::SOUTH | imp::InputEvent::GAMEPAD_BUTTON,
    imp::GamepadButton::WEST | imp::InputEvent::GAMEPAD_BUTTON,
    imp::GamepadButton::EAST | imp::InputEvent::GAMEPAD_BUTTON,
    imp::GamepadButton::RIGHT_STICK | imp::InputEvent::GAMEPAD_BUTTON};

namespace gui {

void ParameterTemplateModal::showCloseButtons() {
	bool isValid = (_hasPress || _hasShoulders || _hasSticks || _hasTriggers)
	               && (_useController || _useMouseKeyboard);
	ImGui::BeginDisabled(!isValid);
	if (ImGui::Button("Create", ImVec2(128.0f, 0.0f))) {
		execute();
		ImGui::CloseCurrentPopup();
	}
	ImGui::EndDisabled();
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(128.0f, 0.0f))) {
		ImGui::CloseCurrentPopup();
	}
}

void ParameterTemplateModal::createPressParameters() {
	vts::Parameter leftPress("MK_LPress");
	vts::Parameter rightPress("MK_RPress");

	if (_useMouseKeyboard) {
		for (const auto id : PRESS_LEFT_KEY_INPUTS) {
			leftPress.addInput(id);
		}
		for (const auto id : PRESS_RIGHT_KEY_INPUTS) {
			rightPress.addInput(id);
		}
	}

	if (_useController) {
		for (const auto id : PRESS_LEFT_GAMEPAD_INPUTS) {
			leftPress.addInput(id);
		}
		for (const auto id : PRESS_RIGHT_GAMEPAD_INPUTS) {
			rightPress.addInput(id);
		}
	}

	SETTINGS.setParameter(leftPress);
	SETTINGS.setParameter(rightPress);
	vts::createParameter(_wsController, leftPress);
	vts::createParameter(_wsController, rightPress);
}

void ParameterTemplateModal::createShoulderParameters() {
	vts::Parameter leftShoulder("MK_LShoulder");
	vts::Parameter rightShoulder("MK_RShoulder");

	if (_useMouseKeyboard) {
		leftShoulder.addInput(VC_Q << 16 | imp::InputEvent::KEY);
		rightShoulder.addInput(VC_E << 16 | imp::InputEvent::KEY);
	}

	if (_useController) {
		leftShoulder.addInput(imp::GamepadButton::LEFT_SHOULDER
		                      | imp::InputEvent::GAMEPAD_BUTTON);
		rightShoulder.addInput(imp::GamepadButton::RIGHT_SHOULDER
		                       | imp::InputEvent::GAMEPAD_BUTTON);
	}

	SETTINGS.setParameter(leftShoulder);
	SETTINGS.setParameter(rightShoulder);
	vts::createParameter(_wsController, leftShoulder);
	vts::createParameter(_wsController, rightShoulder);
}

void ParameterTemplateModal::createStickParameters() {
	vts::Parameter leftStickX("MK_LStickX");
	leftStickX.setBlendMode(vts::BlendMode::BOUNDED_SUM);
	vts::Parameter leftStickY("MK_LStickY");
	leftStickY.setBlendMode(vts::BlendMode::BOUNDED_SUM);

	vts::Parameter rightStickX("MK_RStickX");
	vts::Parameter rightStickY("MK_RStickY");

	if (_useMouseKeyboard) {
		leftStickX.addInput(VC_A << 16 | imp::InputEvent::KEY, true);
		leftStickX.addInput(VC_D << 16 | imp::InputEvent::KEY);
		leftStickY.addInput(VC_W << 16 | imp::InputEvent::KEY);
		leftStickY.addInput(VC_S << 16 | imp::InputEvent::KEY, true);

		rightStickX.addInput(imp::Axis::X | imp::InputEvent::MOUSE_MOVE_REL);
		rightStickY.addInput(imp::Axis::Y | imp::InputEvent::MOUSE_MOVE_REL);
	}

	if (_useController) {
		leftStickX.addInput(imp::Axis::X | imp::InputEvent::GAMEPAD_STICK_LEFT);
		leftStickY.addInput(imp::Axis::Y | imp::InputEvent::GAMEPAD_STICK_LEFT);
		leftStickX.addInput(
		    imp::GamepadButton::DPAD_LEFT | imp::InputEvent::GAMEPAD_BUTTON,
		    true);
		leftStickX.addInput(imp::GamepadButton::DPAD_RIGHT
		                    | imp::InputEvent::GAMEPAD_BUTTON);
		leftStickY.addInput(imp::GamepadButton::DPAD_UP
		                    | imp::InputEvent::GAMEPAD_BUTTON);
		leftStickY.addInput(
		    imp::GamepadButton::DPAD_DOWN | imp::InputEvent::GAMEPAD_BUTTON,
		    true);

		rightStickX.addInput(imp::Axis::X | imp::InputEvent::GAMEPAD_STICK_RIGHT);
		rightStickY.addInput(imp::Axis::Y | imp::InputEvent::GAMEPAD_STICK_RIGHT);
	}

	SETTINGS.setParameter(leftStickX);
	SETTINGS.setParameter(leftStickY);
	SETTINGS.setParameter(rightStickX);
	SETTINGS.setParameter(rightStickY);
	vts::createParameter(_wsController, leftStickX);
	vts::createParameter(_wsController, leftStickY);
	vts::createParameter(_wsController, rightStickX);
	vts::createParameter(_wsController, rightStickY);
}

void ParameterTemplateModal::createTriggerParameters() {
	vts::Parameter leftTrigger("MK_LTrigger");
	vts::Parameter rightTrigger("MK_RTrigger");

	if (_useMouseKeyboard) {
		leftTrigger.addInput(imp::MouseButton::RIGHT | imp::InputEvent::MOUSE_BUTTON);
		rightTrigger.addInput(imp::MouseButton::LEFT | imp::InputEvent::MOUSE_BUTTON);
	}

	if (_useController) {
		leftTrigger.addInput(imp::Side::LEFT | imp::InputEvent::GAMEPAD_TRIGGER);
		rightTrigger.addInput(imp::Side::RIGHT | imp::InputEvent::GAMEPAD_TRIGGER);
	}

	SETTINGS.setParameter(leftTrigger);
	SETTINGS.setParameter(rightTrigger);
	vts::createParameter(_wsController, leftTrigger);
	vts::createParameter(_wsController, rightTrigger);
}

void ParameterTemplateModal::execute() {
	if (_hasPress) {
		createPressParameters();
	}
	if (_hasShoulders) {
		createShoulderParameters();
	}
	if (_hasSticks) {
		createStickParameters();
	}
	if (_hasTriggers) {
		createTriggerParameters();
	}
}

ParameterTemplateModal::ParameterTemplateModal(ws::IController& wsController) :
    _wsController(wsController),
    _hasPress(true),
    _hasShoulders(true),
    _hasSticks(true),
    _hasTriggers(true),
    _useController(true),
    _useMouseKeyboard(true),
    _templateSelector("##template-selector", TEMPLATES) {}

void ParameterTemplateModal::show() {
	centerNextWindow();
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoNav
	                               | ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::SeparatorText("Template");

		ImGui::Text("Select the type of device your VTuber model will control.");
		_templateSelector.show();

		ImGui::SeparatorText("Actions");

		ImGui::Text(
		    "Select which parts of the game controller your model will be able to "
		    "interact with.");

		ImGui::Checkbox("##action-press", &_hasPress);
		ImGui::SameLine();
		ImGui::Text("Press (pushing on the sides)");

		ImGui::Checkbox("##action-shoulders", &_hasShoulders);
		ImGui::SameLine();
		ImGui::Text("Shoulders (LB, RB)");

		ImGui::Checkbox("##action-sticks", &_hasSticks);
		ImGui::SameLine();
		ImGui::Text("Sticks (LS, RS)");

		ImGui::Checkbox("##action-triggers", &_hasTriggers);
		ImGui::SameLine();
		ImGui::Text("Triggers (LT, RT)");

		ImGui::SeparatorText("Inputs");

		ImGui::Text(
		    "Select which of your physical devices will send input to the virtual "
		    "controller.");

		ImGui::Checkbox("##device-controller", &_useController);
		ImGui::SameLine();
		ImGui::Text("Controller");

		ImGui::Checkbox("##device-mouse-keyboard", &_useMouseKeyboard);
		ImGui::SameLine();
		ImGui::Text("Mouse & Keyboard");

		showCloseButtons();

		ImGui::EndPopup();
	}
}

}  // namespace gui
