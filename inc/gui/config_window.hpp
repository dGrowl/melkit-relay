#ifndef GUI_CONFIG_WINDOW_HPP_
#define GUI_CONFIG_WINDOW_HPP_

#include <cstddef>

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_video.h>

#include "gui/config_parameter_panel.hpp"
#include "gui/config_settings_panel.hpp"
#include "gui/theme_color_modal.hpp"
#include "impulse/processor.hpp"
#include "pad/manager.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

class ConfigWindow {
private:
	SDL_Window* _window;

	ConfigSettingsPanel  _settingsPanel;
	ConfigParameterPanel _parameterPanel;

	ThemeColorModal _themeColorModal;

	void showMenuBar();

public:
	ConfigWindow(pad::Manager&          gamepadManager,
	             imp::Processor&        impulseProcessor,
	             ws::IController&       wsController,
	             vts::ParameterManager& paramManager);

	bool         isOpen() const;
	int          open(SDL_GPUDevice* gpu);
	SDL_WindowID id() const;
	void         close(SDL_GPUDevice* gpu);
	void         render(SDL_GPUDevice* gpu);
	void         setActiveGamepadIndex(size_t activeIndex);
};

}  // namespace gui

#endif  // GUI_CONFIG_WINDOW_HPP_
