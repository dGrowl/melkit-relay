#ifndef GUI_ADD_INPUT_MODAL_HPP_
#define GUI_ADD_INPUT_MODAL_HPP_

#include <string>

#include "gui/combo_box.hpp"
#include "vts/parameter.hpp"

namespace gui {

class AddInputModal {
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

	vts::TargetTag getMouseAxisTag() const;
	vts::TargetTag getMouseButtonTag() const;
	vts::TargetTag getMouseWheelTag() const;

	vts::TargetTag getGamepadButtonTag() const;
	vts::TargetTag getGamepadStickActionTag(const bool isLeftStick) const;
	vts::TargetTag getGamepadTriggerTag() const;

	vts::InputId buildInputId() const;

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
	AddInputModal(vts::Parameter& editingParameter);
	void show();

	constexpr static const char* NAME = "Add Input";
};

}  // namespace gui

#endif  // GUI_ADD_INPUT_MODAL_HPP_
