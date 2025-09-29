#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>

#include "core/app.hpp"
#include "core/meta.hpp"

static constexpr auto APP_NAME       = core::PLUGIN_NAME;
static constexpr auto APP_VERSION    = "0.1.0-alpha";
static constexpr auto APP_IDENTIFIER = "io.github.dgrowl.relay.melkit";

int init() {
	SDL_SetAppMetadata(APP_NAME, APP_VERSION, APP_IDENTIFIER);
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
