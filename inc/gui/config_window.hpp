#ifndef GUI_CONFIG_WINDOW_HPP_
#define GUI_CONFIG_WINDOW_HPP_

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_sdlgpu3.h"
#include "imgui/imgui.h"

#include "gui/config_settings_panel.hpp"
#include "gui/delete_parameters_modal.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "gui/parameter_template_modal.hpp"
#include "pad/manager.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

class ConfigWindow {
private:
	pad::Manager&          _gamepadManager;
	vts::Parameter&        _editingParameter;
	vts::ParameterManager& _paramManager;
	ws::IController&       _wsController;

	SDL_Window* _window;

	ConfigSettingsPanel    _settingsPanel;
	DeleteParametersModal  _deleteParametersModal;
	EditParameterModal     _editParameterModal;
	ParameterTemplateModal _parameterTemplateModal;

	bool showCreateParameter();
	void showParameterControls();
	void showParameterData();
	void showParameterPanel();
	void showParameters();

	void showParameterModals();

public:
	ConfigWindow(pad::Manager&          gamepadManager,
	             ws::IController&       wsController,
	             vts::ParameterManager& paramManager);

	bool         isOpen() const;
	int          open(SDL_GPUDevice* gpu);
	SDL_WindowID id() const;
	void         close(SDL_GPUDevice* gpu);
	void         render(SDL_GPUDevice* gpu);
	void         setActiveGamepadIndex(const size_t activeIndex);
};

}  // namespace gui

#endif  // GUI_CONFIG_WINDOW_HPP_
