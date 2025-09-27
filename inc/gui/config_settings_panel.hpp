#ifndef GUI_CONFIG_SETTINGS_PANEL_HPP_
#define GUI_CONFIG_SETTINGS_PANEL_HPP_

#include "gui/combo_box.hpp"
#include "gui/set_mouse_bounds_modal.hpp"
#include "pad/manager.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace gui {

constexpr Uint8 MAX_URL_LENGTH = 128;

class ConfigSettingsPanel {
private:
	pad::Manager&    _gamepadManager;
	vts::Processor&  _impulseProcessor;
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
	                    vts::Processor&  impulseProcessor,
	                    vts::Parameter&  editingParameter,
	                    ws::IController& wsController);

	void show();
	void setActiveGamepadIndex(const size_t activeIndex);
};

}  // namespace gui

#endif  // GUI_CONFIG_SETTINGS_PANEL_HPP_
