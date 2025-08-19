#ifndef TRAY_ICON_HPP_
#define TRAY_ICON_HPP_

#include <SDL3/SDL.h>

namespace gui {

class TrayIcon {
private:
	SDL_Tray* _tray;
	SDL_TrayMenu* _menu;

	SDL_TrayEntry* _configEntry;
	SDL_TrayEntry* _quitEntry;

public:
	TrayIcon();
	void destroy();
	void setQuitCallback(SDL_TrayCallback callback, void* userdata);
	void setConfigCallback(SDL_TrayCallback callback, void* userdata);
};

}  // namespace gui

#endif  // TRAY_ICON_HPP_
