#include <SDL3/SDL_log.h>

#include "mnk/event.hpp"
#include "mnk/monitor.hpp"

namespace mnk {

Monitor::Monitor() :
    _thread(&Monitor::threadFn, this) {}

Monitor::~Monitor() {
	if (_thread.joinable()) {
		_thread.join();
	}
}

void Monitor::buildKeyDown(SDL_UserEvent& userEvent,
                           uiohook_event* const hookEvent) {
	userEvent.code = InputCode::KEY_DOWN;
	auto* keyboard = new KeyboardData();
	keyboard->keycode = hookEvent->data.keyboard.keycode;
	userEvent.data1 = keyboard;
}

void Monitor::buildKeyUp(SDL_UserEvent& userEvent,
                         uiohook_event* const hookEvent) {
	userEvent.code = InputCode::KEY_UP;
	auto* keyboard = new KeyboardData();
	keyboard->keycode = hookEvent->data.keyboard.keycode;
	userEvent.data1 = keyboard;
}

void Monitor::buildMouseMove(SDL_UserEvent& userEvent,
                             uiohook_event* const hookEvent) {
	userEvent.code = InputCode::MOUSE_MOVE;
	auto* mouse = new MouseData();
	mouse->x = hookEvent->data.mouse.x;
	mouse->y = hookEvent->data.mouse.y;
	userEvent.data1 = mouse;
}

void Monitor::buildMouseClick(SDL_UserEvent& userEvent,
                              uiohook_event* const hookEvent) {
	userEvent.code = InputCode::MOUSE_CLICK;
	auto* mouse = new MouseData();
	mouse->button = hookEvent->data.mouse.button;
	userEvent.data1 = mouse;
}

void Monitor::buildMouseRelease(SDL_UserEvent& userEvent,
                                uiohook_event* const hookEvent) {
	userEvent.code = InputCode::MOUSE_RELEASE;
	auto* mouse = new MouseData();
	mouse->button = hookEvent->data.mouse.button;
	userEvent.data1 = mouse;
}

void Monitor::handleEvent(uiohook_event* const event) {
	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = Event::INPUT;
	switch (event->type) {
		case EVENT_KEY_PRESSED:
			buildKeyDown(sdlEvent.user, event);
			break;
		case EVENT_KEY_RELEASED:
			buildKeyUp(sdlEvent.user, event);
			break;
		case EVENT_MOUSE_DRAGGED:
		case EVENT_MOUSE_MOVED:
			buildMouseMove(sdlEvent.user, event);
			break;
		case EVENT_MOUSE_PRESSED:
		case EVENT_MOUSE_CLICKED:
			buildMouseClick(sdlEvent.user, event);
			break;
		case EVENT_MOUSE_RELEASED:
			buildMouseRelease(sdlEvent.user, event);
			break;
		default:
			return;
	}

	SDL_PushEvent(&sdlEvent);
}

void Monitor::stop() {
	hook_stop();
}

void Monitor::threadFn() {
	hook_set_dispatch_proc(handleEvent);

	int status = hook_run();
	if (status != UIOHOOK_SUCCESS) {
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Failed to start uiohook\n");
	}
}

}  // namespace mnk
