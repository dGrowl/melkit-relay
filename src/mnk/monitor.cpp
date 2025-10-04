#include "mnk/monitor.hpp"

#include <cstdint>
#include <libuiohook/uiohook.h>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>

#include "impulse/code.hpp"
#include "mnk/event.hpp"

template <typename T>
void* unsignedToPointer(const T x) {
	return reinterpret_cast<void*>(static_cast<uintptr_t>(x));
}

template <typename T>
void* signedToPointer(const T x) {
	return reinterpret_cast<void*>(static_cast<intptr_t>(x));
}

namespace mnk {

Monitor::Monitor() :
    _thread(&Monitor::threadFn, this) {}

Monitor::~Monitor() {
	if (_thread.joinable()) {
		_thread.join();
	}
}

void Monitor::buildKeyDown(SDL_UserEvent&       userEvent,
                           uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::KEY_DOWN;
	userEvent.data1 = unsignedToPointer(hookEvent->data.keyboard.keycode);
}

void Monitor::buildKeyUp(SDL_UserEvent&       userEvent,
                         uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::KEY_UP;
	userEvent.data1 = unsignedToPointer(hookEvent->data.keyboard.keycode);
}

void Monitor::buildMouseMove(SDL_UserEvent&       userEvent,
                             uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::MOUSE_MOVE;
	userEvent.data1 = signedToPointer(hookEvent->data.mouse.x);
	userEvent.data2 = signedToPointer(hookEvent->data.mouse.y);
}

void Monitor::buildMouseClick(SDL_UserEvent&       userEvent,
                              uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::MOUSE_CLICK;
	userEvent.data1 = unsignedToPointer(hookEvent->data.mouse.button << 16);
}

void Monitor::buildMouseRelease(SDL_UserEvent&       userEvent,
                                uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::MOUSE_RELEASE;
	userEvent.data1 = unsignedToPointer(hookEvent->data.mouse.button << 16);
}

void Monitor::buildMouseWheel(SDL_UserEvent&       userEvent,
                              uiohook_event* const hookEvent) {
	userEvent.code  = imp::DeviceAction::MOUSE_WHEEL;
	userEvent.data1 = signedToPointer(hookEvent->data.wheel.rotation);
}

void Monitor::handleEvent(uiohook_event* const event) {
	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = Event::ACTION;
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
			buildMouseClick(sdlEvent.user, event);
			break;
		case EVENT_MOUSE_RELEASED:
			buildMouseRelease(sdlEvent.user, event);
			break;
		case EVENT_MOUSE_WHEEL:
			buildMouseWheel(sdlEvent.user, event);
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
