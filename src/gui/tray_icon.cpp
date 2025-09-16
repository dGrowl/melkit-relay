#include <SDL3_image/SDL_image.h>

#include "gui/image.hpp"
#include "gui/tray_icon.hpp"

namespace gui {

TrayIcon::TrayIcon() :
    _tray(nullptr),
    _menu(nullptr),
    _configEntry(nullptr),
    _quitEntry(nullptr) {
	Surface image(ICON_PATH);
	_tray = SDL_CreateTray(image, "Relay");
	_menu = SDL_CreateTrayMenu(_tray);
	_configEntry =
	    SDL_InsertTrayEntryAt(_menu, -1, "Configure", SDL_TRAYENTRY_BUTTON);
	_quitEntry = SDL_InsertTrayEntryAt(_menu, -1, "Quit", SDL_TRAYENTRY_BUTTON);
}

void TrayIcon::destroy() {
	SDL_DestroyTray(_tray);
}

void TrayIcon::setConfigCallback(SDL_TrayCallback callback, void* userdata) {
	SDL_SetTrayEntryCallback(_configEntry, callback, userdata);
}

void TrayIcon::setQuitCallback(SDL_TrayCallback callback, void* userdata) {
	SDL_SetTrayEntryCallback(_quitEntry, callback, userdata);
}

}  // namespace gui
