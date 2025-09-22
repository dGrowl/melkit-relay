#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>

#include "core/app.hpp"

int init() {
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
		return 1;
	}
	return 0;
}

int main() {
	if (init() != 0) {
		return 1;
	}
	core::App app;
	if (app.init() != 0) {
		return 2;
	}
	app.run();
	return 0;
}
