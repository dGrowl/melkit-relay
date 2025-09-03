#ifndef CORE_APP_HPP_
#define CORE_APP_HPP_

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_tray.h>

#include "gui/config_window.hpp"
#include "gui/tray_icon.hpp"
#include "mnk/monitor.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/client.hpp"

namespace core {

class App {
private:
	bool _alive;
	SDL_GPUDevice* _gpu;

	vts::ParameterManager _params;
	mnk::Monitor _mnkMonitor;
	ws::Client _wsClient;

	gui::ConfigWindow _config;
	gui::TrayIcon _icon;

	void handleVtsAuthenticationFailure();
	void handleVtsAuthenticationSuccess();
	void handleVtsAuthenticationToken(SDL_UserEvent& event);
	void handleVtsInputParameterList(SDL_UserEvent& event);
	void handleVtsParameterCreation();
	void handleVtsParameterDeletion();

	void loadParameterSettings();

public:
	App();
	~App();

	int init();
	void quit();
	void run();
	void openConfig();
	void stopMouseKeyboard();
	void stopWs();
	void handleEvent(SDL_Event& event);
	void handleVtsMessage(SDL_UserEvent& event);
	void handleWindowClose(SDL_Event& event);

	static void openConfigCallback(void* userdata, SDL_TrayEntry*);
	static void quitCallback(void* userdata, SDL_TrayEntry*);
};

}  // namespace core

#endif  // CORE_APP_HPP_
