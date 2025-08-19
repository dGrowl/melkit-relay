#ifndef CONFIG_WINDOW_HPP_
#define CONFIG_WINDOW_HPP_

#include <SDL3/SDL.h>
#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlgpu3.h>
#include <imgui/imgui.h>

#include "ws/controller.hpp"

namespace gui {

constexpr Uint8 MAX_URL_LENGTH = 128;

class ConfigWindow {
private:
	const char* _title;
	int _height;
	int _width;
	SDL_FColor _clearColor;

	ws::IController& _wsController;

	char _urlBuffer[MAX_URL_LENGTH];

	SDL_Window* _window;
	SDL_WindowFlags _flags;

public:
	ConfigWindow(ws::IController& wsController);
	int open(SDL_GPUDevice* gpu);
	void close(SDL_GPUDevice* gpu);
	void render(SDL_GPUDevice* gpu);
	bool isOpen() const;
	SDL_WindowID id() const;

	void showVtsConnection();
};

}  // namespace gui

#endif  // CONFIG_WINDOW_HPP_
