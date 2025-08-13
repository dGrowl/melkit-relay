#ifndef CONFIG_WINDOW_HPP_
#define CONFIG_WINDOW_HPP_

#include <SDL3/SDL.h>

class ConfigWindow {
private:
	bool _alive;
	const char* _title;
	int _height;
	int _width;
	SDL_Event _event;
	SDL_FColor _clearColor;
	SDL_Window* _window;
	SDL_WindowFlags _flags;
	void destroy(SDL_GPUDevice* gpu);

public:
	ConfigWindow();
	int create(SDL_GPUDevice* gpu);
};

#endif  // CONFIG_WINDOW_HPP_
