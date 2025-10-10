#include "gui/tray_icon.hpp"

#include <SDL3/SDL_tray.h>

#include "core/settings.hpp"
#include "gui/image.hpp"

namespace gui {

TrayIcon::TrayIcon() :
    _tray(nullptr),
    _menu(nullptr),
    _configEntry(nullptr),
    _quitEntry(nullptr) {
	_tray = SDL_CreateTray(nullptr, "Relay");
	loadImage(SETTINGS.getThemeHueShift());
	_menu = SDL_CreateTrayMenu(_tray);
	_configEntry =
	    SDL_InsertTrayEntryAt(_menu, -1, "Configure", SDL_TRAYENTRY_BUTTON);
	_quitEntry = SDL_InsertTrayEntryAt(_menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);
}

void TrayIcon::destroy() {
	SDL_DestroyTray(_tray);
}

void TrayIcon::loadImage(float hueShift) {
	Surface image = Surface::APP_ICON();
	if (hueShift != 0.0F) {
		image.shiftHue(hueShift);
	}
	SDL_SetTrayIcon(_tray, image);
}

void TrayIcon::setConfigCallback(SDL_TrayCallback callback, void* userdata) {
	SDL_SetTrayEntryCallback(_configEntry, callback, userdata);
}

void TrayIcon::setQuitCallback(SDL_TrayCallback callback, void* userdata) {
	SDL_SetTrayEntryCallback(_quitEntry, callback, userdata);
}

}  // namespace gui
