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

	vts::InputId   getMouseAxisId() const;
	vts::InputId   getMouseButtonId() const;
	vts::InputData buildInputData() const;

	void showCloseButtons();
	void showDeviceSelector();

	void showMouseAxisSelector();
	void showMouseButtonSelector();
	void showMouseControls();
	void showMouseEventSelector();

	void showKeyboardControls();

public:
	AddInputModal(vts::Parameter& editingParameter);
	void show();

	constexpr static const char* NAME = "Add Input";
};

}  // namespace gui

#endif  // GUI_ADD_INPUT_MODAL_HPP_
