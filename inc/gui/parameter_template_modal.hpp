#ifndef GUI_PARAMETER_TEMPLATE_MODAL_
#define GUI_PARAMETER_TEMPLATE_MODAL_

#include "gui/combo_box.hpp"

namespace gui {

class ParameterTemplateModal {
private:
	bool _hasPress;
	bool _hasShoulders;
	bool _hasSticks;
	bool _hasTriggers;

	bool _useController;
	bool _useKeyboard;
	bool _useMouse;

	ComboBox _templateSelector;

	void showCloseButtons();

public:
	ParameterTemplateModal();

	void show();

	constexpr static const char* NAME = "Add Template";
};

}  // namespace gui

#endif  // GUI_PARAMETER_TEMPLATE_MODAL_
