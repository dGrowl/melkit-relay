#ifndef GUI_ADD_IMPULSE_MODAL_HPP_
#define GUI_ADD_IMPULSE_MODAL_HPP_

#include <string>

#include "imgui/imgui.h"

#include "gui/add_impulse_modal.hpp"
#include "gui/combo_box.hpp"
#include "impulse/code.hpp"
#include "vts/parameter.hpp"

namespace gui {

class AddImpulseModal {
private:
	std::string _selectedKeyName;
	ImGuiKey    _selectedKey;

	vts::Parameter& _editingParameter;

	ComboBox _deviceSelector;

	ComboBox _mouseAxisSelector;
	ComboBox _mouseButtonSelector;
	ComboBox _mouseEventSelector;
	ComboBox _mouseWheelSelector;

	ComboBox _gamepadButtonSelector;
	ComboBox _gamepadEventSelector;
	ComboBox _gamepadStickActionSelector;
	ComboBox _gamepadTriggerSelector;

	[[nodiscard]] imp::TargetTag getMouseAxisTag() const;
	[[nodiscard]] imp::TargetTag getMouseButtonTag() const;
	[[nodiscard]] imp::TargetTag getMouseWheelTag() const;

	[[nodiscard]] imp::Code      getGamepadStickActionCode(bool isLeftStick) const;
	[[nodiscard]] imp::TargetTag getGamepadButtonTag() const;
	[[nodiscard]] imp::TargetTag getGamepadTriggerTag() const;

	[[nodiscard]] imp::Code buildImpulseCode() const;

	void showCloseButtons();
	void showDeviceSelector();

	void showMouseAxisSelector();
	void showMouseButtonSelector();
	void showMouseControls();
	void showMouseEventSelector();
	void showMouseWheelSelector();

	void showKeyboardControls();

	void showGamepadButtonSelector();
	void showGamepadControls();
	void showGamepadEventSelector();
	void showGamepadStickActionSelector();
	void showGamepadTriggerSelector();

public:
	explicit AddImpulseModal(vts::Parameter& editingParameter);
	void show();

	constexpr static const char* NAME = "Add Input";
};

}  // namespace gui

#endif  // GUI_ADD_IMPULSE_MODAL_HPP_
