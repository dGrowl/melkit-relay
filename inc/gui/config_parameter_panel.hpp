#ifndef GUI_CONFIG_PARAMETER_PANEL_HPP_
#define GUI_CONFIG_PARAMETER_PANEL_HPP_

#include "gui/delete_parameters_modal.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "gui/parameter_template_modal.hpp"
#include "pad/manager.hpp"
#include "vts/parameter.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

class ConfigParameterPanel {
private:
	vts::Parameter&        _editingParameter;
	vts::ParameterManager& _parameterManager;
	ws::IController&       _wsController;

	DeleteParametersModal  _deleteParametersModal;
	EditParameterModal     _editParameterModal;
	ParameterTemplateModal _parameterTemplateModal;

	bool showCreateOption();
	void showControls();
	void showData();
	void showModals();
	void showParameters();

public:
	ConfigParameterPanel(vts::Parameter&        editingParameter,
	                     vts::ParameterManager& parameterManager,
	                     ws::IController&       wsController);

	void show();
};

}  // namespace gui

#endif  // GUI_CONFIG_PARAMETER_PANEL_HPP_
