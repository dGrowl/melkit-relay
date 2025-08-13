#ifndef CONFIG_WINDOW_HPP_
#define CONFIG_WINDOW_HPP_

#include <SDL3/SDL.h>

#include <imgui/backends/imgui_impl_sdl3.h>
#include <imgui/backends/imgui_impl_sdlgpu3.h>
#include <imgui/imgui.h>

class ConfigWindow {
private:
	bool _showDemoWindow;
	const char* _title;
	int _height;
	int _width;
	SDL_FColor _clearColor;
	SDL_Window* _window;
	SDL_WindowFlags _flags;

public:
	ConfigWindow();
	int open(SDL_GPUDevice* gpu);
	void close(SDL_GPUDevice* gpu);
	void render(SDL_GPUDevice* gpu);
	bool isOpen() const;
	SDL_WindowID id() const;
};

#endif  // CONFIG_WINDOW_HPP_
