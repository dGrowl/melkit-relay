#ifndef GUI_DELETE_PARAMETERS_MODAL_
#define GUI_DELETE_PARAMETERS_MODAL_

#include <string>
#include <unordered_map>

#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

class DeleteParametersModal {
private:
	vts::ParameterManager& _parameterManager;
	ws::IController&       _wsController;

	bool                                  _allSelected = false;
	std::unordered_map<std::string, bool> _selectedState;

public:
	DeleteParametersModal(vts::ParameterManager& parameterManager,
	                      ws::IController&       wsController);
	void show();

	constexpr static const char* NAME = "Delete Parameters";
};

}  // namespace gui

#endif  // GUI_DELETE_PARAMETERS_MODAL_
