#include <SDL3/SDL.h>

const SDL_InitFlags SDL_FLAGS = (SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

const char* WINDOW_TITLE = "SDL3 Starter";
const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;
const SDL_WindowFlags WINDOW_FLAGS =
    (SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY);

int main(int, char*[]) {
	if (!SDL_Init(SDL_FLAGS)) {
		return 1;
	}

	const SDL_DisplayID primaryDisplay = SDL_GetPrimaryDisplay();
	const float mainScale = SDL_GetDisplayContentScale(primaryDisplay);
	SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE,
	                                      (int)(WINDOW_WIDTH * mainScale),
	                                      (int)(WINDOW_HEIGHT * mainScale),
	                                      WINDOW_FLAGS);
	if (window == nullptr) {
		return 2;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	bool alive = true;
	SDL_Event event;
	while (alive) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				alive = false;
			}
		}
		SDL_Delay(100);
	}

	return 0;
}
