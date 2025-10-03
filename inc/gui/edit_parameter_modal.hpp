#ifndef GUI_EDIT_PARAMETER_MODAL_
#define GUI_EDIT_PARAMETER_MODAL_
#include <array>

#include "core/meta.hpp"
#include "gui/add_impulse_modal.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

constexpr size_t MAX_NAME_BUFFER_LENGTH =
    core::MAX_PARAMETER_LENGTH + sizeof('\0');
constexpr size_t N_HISTORY_SAMPLES = 128;

namespace gui {

class EditParameterModal {
private:
	ws::IController& _wsController;
	vts::Parameter&  _editingParameter;

	AddImpulseModal _addImpulseModal;
	ComboBox        _blendModeSelector;

	char        _nameFieldBuffer[MAX_NAME_BUFFER_LENGTH];
	std::string _initialName;

	std::array<float, N_HISTORY_SAMPLES> _outputHistory;
	int                                  _outputOffset;

	imp::Code _impulseCodeToDelete;

	void showAddImpulse();
	void showImpulses();
	void showMeta();
	void showOutput();

	int        restrictInputName(ImGuiInputTextCallbackData* data);
	static int inputNameCallback(ImGuiInputTextCallbackData* data);
	void       checkDeleteImpulse();
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
