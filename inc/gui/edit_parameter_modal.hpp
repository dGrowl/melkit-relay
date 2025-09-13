#ifndef GUI_EDIT_PARAMETER_MODAL_
#define GUI_EDIT_PARAMETER_MODAL_

#include "gui/add_input_modal.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

constexpr size_t MAX_NAME_BUFFER_LENGTH = 33;

namespace gui {

class EditParameterModal {
private:
	ws::IController& _wsController;
	vts::Parameter&  _editingParameter;

	AddInputModal _addInputModal;
	ComboBox      _blendModeSelector;

	char        _nameInputBuffer[MAX_NAME_BUFFER_LENGTH];
	std::string _initialName;

	vts::InputId _inputIdToDelete;

	void showAddInput();
	void showInputs();
	void showMeta();
	void showOutput();

	int        restrictInputName(ImGuiInputTextCallbackData* data);
	static int inputNameCallback(ImGuiInputTextCallbackData* data);
	void       checkDeleteInput();
	void       save();
	void       updateBlendMode();

public:
	EditParameterModal(ws::IController& wsController,
	                   vts::Parameter&  editingParameter);

	void refresh();
	void show();

	constexpr static const char* NAME = "Edit Parameter";
};

}  // namespace gui

#endif  // GUI_EDIT_PARAMETER_MODAL_
