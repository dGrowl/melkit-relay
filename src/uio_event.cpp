#include "uio_event.hpp"

const int N_UIO_EVENTS = 4;

void allocateUIOEvents() {
	SDL_RegisterEvents(N_UIO_EVENTS);
}

void uioHookCallback(uiohook_event* const event) {
	Uint32 type = 0;
	UIO_Event data;
	switch (event->type) {
		case EVENT_KEY_PRESSED:
			type = UIO_EVENT_KEY_DOWN;
			data.key.keycode = event->data.keyboard.keycode;
			break;
		case EVENT_KEY_RELEASED:
			type = UIO_EVENT_KEY_UP;
			data.key.keycode = event->data.keyboard.keycode;
			break;
		case EVENT_MOUSE_MOVED:
			type = UIO_EVENT_MOUSE_MOVE;
			data.mouse.x = event->data.mouse.x;
			data.mouse.y = event->data.mouse.y;
			break;
		case EVENT_MOUSE_CLICKED:
			type = UIO_EVENT_MOUSE_CLICK;
			data.mouse.button = event->data.mouse.button;
			break;
		default:
			return;
	}

	data.type = type;

	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = type;
	sdlEvent.user.data1 = SDL_malloc(sizeof(UIO_Event));
	SDL_memcpy(sdlEvent.user.data1, &data, sizeof(UIO_Event));

	SDL_PushEvent(&sdlEvent);
}

int uioHookThreadFn(void*) {
	hook_set_dispatch_proc(uioHookCallback);

	int status = hook_run();
	if (status != UIOHOOK_SUCCESS) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Failed to start uiohook\n");
	}

	return status;
}
