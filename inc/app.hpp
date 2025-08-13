#ifndef APP_HPP_
#define APP_HPP_

#include <SDL3/SDL.h>

#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlgpu3.h>
#include <imgui/imgui.h>

#include "config_window.hpp"
#include "tray_icon.hpp"

class App {
private:
	bool _alive;
	SDL_GPUDevice* _gpu;
	ConfigWindow _config;
	TrayIcon _icon;

public:
	App();
	~App();
	int init();
	void quit();
	void run();
	void openConfig();

	static void openConfigCallback(void* userdata, SDL_TrayEntry*);
	static void quitCallback(void* userdata, SDL_TrayEntry*);
};

#endif  // APP_HPP_
