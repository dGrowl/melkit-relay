#ifndef GUI_EDIT_PARAMETER_MODAL_
#define GUI_EDIT_PARAMETER_MODAL_

#include "gui/add_input_modal.hpp"
#include "vts/parameter.hpp"

namespace gui {

class EditParameterModal {
private:
	vts::Parameter& _editingParameter;

	AddInputModal _addInputModal;

	void showAddInput();
	void showInputs();
	void showOutput();

public:
	EditParameterModal(vts::Parameter& editingParameter);
	void show();

	constexpr static const char* NAME = "Edit Parameter";
};

}  // namespace gui

#endif  // GUI_EDIT_PARAMETER_MODAL_
