#ifndef GUI_PARAMETER_TEMPLATE_MODAL_
#define GUI_PARAMETER_TEMPLATE_MODAL_

#include "gui/combo_box.hpp"
#include "ws/controller.hpp"

namespace gui {

class ParameterTemplateModal {
private:
	ws::IController& _wsController;

	bool _hasPress;
	bool _hasShoulders;
	bool _hasSticks;
	bool _hasTriggers;

	bool _useController;
	bool _useMouseKeyboard;

	ComboBox _templateSelector;

	void showCloseButtons();

	void createPressParameters();
	void createShoulderParameters();
	void createStickParameters();
	void createTriggerParameters();
	void execute();

public:
	ParameterTemplateModal(ws::IController& wsController);

	void show();

	constexpr static const char* NAME = "Add Template";
};

}  // namespace gui

#endif  // GUI_PARAMETER_TEMPLATE_MODAL_
