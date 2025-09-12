#ifndef GUI_CONFIG_WINDOW_HPP_
#define GUI_CONFIG_WINDOW_HPP_

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include "imgui/backends/imgui_impl_sdl3.h"
#include "imgui/backends/imgui_impl_sdlgpu3.h"
#include "imgui/imgui.h"

#include "gui/delete_parameters_modal.hpp"
#include "gui/edit_parameter_modal.hpp"
#include "pad/manager.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

constexpr Uint8 MAX_URL_LENGTH = 128;

constexpr Uint8 MAX_PARAM_NAME_LENGTH = 128;

class ConfigWindow {
private:
	pad::Manager&          _gamepadManager;
	vts::Parameter&        _editingParameter;
	vts::ParameterManager& _paramManager;
	ws::IController&       _wsController;

	char     _newParamNameBuffer[MAX_PARAM_NAME_LENGTH];
	char     _urlBuffer[MAX_URL_LENGTH];
	ComboBox _gamepadSelector;

	SDL_Window* _window;

	DeleteParametersModal _deleteParametersModal;
	EditParameterModal    _editParameterModal;

	void showCreateParameter();
	void showDeleteParameters();
	void showParameterControls();
	void showParameterData();
	void showParameterPanel();
	void showParameters();

	void showGamepadSettings();
	void showSettingsPanel();
	void showVtsSettings();

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
