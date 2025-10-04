#ifndef GUI_CONFIG_PARAMETER_PANEL_HPP_
#define GUI_CONFIG_PARAMETER_PANEL_HPP_

#include <cstddef>
#include <string>
#include <unordered_set>

#include "core/meta.hpp"
#include "gui/delete_parameters_modal.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "gui/parameter_template_modal.hpp"
#include "vts/parameter.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

constexpr size_t MAX_FILTER_LENGTH = core::MAX_PARAMETER_LENGTH + sizeof('\0');

namespace gui {

class ConfigParameterPanel {
private:
	vts::Parameter&        _editingParameter;
	vts::ParameterManager& _parameterManager;
	ws::IController&       _wsController;

	DeleteParametersModal  _deleteParametersModal;
	EditParameterModal     _editParameterModal;
	ParameterTemplateModal _parameterTemplateModal;

	std::unordered_set<std::string> _filteredParameterNames;
	char                            _filterBuffer[MAX_FILTER_LENGTH];

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
