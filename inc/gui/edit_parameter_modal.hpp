#ifndef GUI_EDIT_PARAMETER_MODAL_
#define GUI_EDIT_PARAMETER_MODAL_

#include "gui/add_input_modal.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace gui {

class EditParameterModal {
private:
	ws::IController& _wsController;
	vts::Parameter& _editingParameter;

	AddInputModal _addInputModal;

	void showAddInput();
	void showInputs();
	void showOutput();

public:
	EditParameterModal(ws::IController& wsController,
	                   vts::Parameter& editingParameter);
	void show();

	constexpr static const char* NAME = "Edit Parameter";
};

}  // namespace gui

#endif  // GUI_EDIT_PARAMETER_MODAL_
