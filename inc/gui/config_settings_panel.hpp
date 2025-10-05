#ifndef GUI_CONFIG_SETTINGS_PANEL_HPP_
#define GUI_CONFIG_SETTINGS_PANEL_HPP_

#include <cstddef>

#include <SDL3/SDL_stdinc.h>

#include "gui/combo_box.hpp"
#include "gui/set_mouse_bounds_modal.hpp"
#include "impulse/processor.hpp"
#include "pad/manager.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"
namespace gui {

constexpr Uint8 MAX_URL_LENGTH = 128;

class ConfigSettingsPanel {
private:
	pad::Manager&    _gamepadManager;
	imp::Processor&  _impulseProcessor;
	ws::IController& _wsController;

	SetMouseBoundsModal _setMouseBoundsModal;

	char     _urlBuffer[MAX_URL_LENGTH];
	ComboBox _gamepadSelector;
	int      _mouseSensitivity;

	void showGamepadSettings();
	void showMouseMotionSettings();
	void showMousePositionSettings();
	void showSettingsPanel();
	void showVtsSettings();

	void showModals();

public:
	ConfigSettingsPanel(pad::Manager&    gamepadManager,
	                    imp::Processor&  impulseProcessor,
	                    vts::Parameter&  editingParameter,
	                    ws::IController& wsController);

	void show();
	void setActiveGamepadIndex(size_t activeIndex);
};

}  // namespace gui

#endif  // GUI_CONFIG_SETTINGS_PANEL_HPP_
