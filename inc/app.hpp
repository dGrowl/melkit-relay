#ifndef APP_HPP_
#define APP_HPP_

#include <SDL3/SDL.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlgpu3.h>
#include <imgui/imgui.h>

#include "gui/config_window.hpp"
#include "gui/tray_icon.hpp"
#include "input_state.hpp"
#include "uio_event.hpp"
#include "ws/client.hpp"

class App {
private:
	bool _alive;
	InputState _input;

	SDL_GPUDevice* _gpu;
	SDL_Thread* _uioThread;
	ws::Client _wsClient;

	gui::ConfigWindow _config;
	gui::TrayIcon _icon;

public:
	App();
	~App();
	int init();
	void quit();
	void run();
	void openConfig();
	void stopUio();
	void stopWs();
	void handleEvent(SDL_Event& event);
	void handleVtsMessage(SDL_UserEvent& event);
	void handleWindowClose(SDL_Event& event);

	static void openConfigCallback(void* userdata, SDL_TrayEntry*);
	static void quitCallback(void* userdata, SDL_TrayEntry*);
};

#endif  // APP_HPP_
