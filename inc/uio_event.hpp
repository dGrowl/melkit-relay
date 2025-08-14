#ifndef UIO_EVENT_HPP_
#define UIO_EVENT_HPP_

#include <SDL3/SDL.h>
#include <libuiohook/uiohook.h>

enum : Uint32 {
	UIO_EVENT_KEY_DOWN = SDL_EVENT_USER,
	UIO_EVENT_KEY_UP = UIO_EVENT_KEY_DOWN + 1,
	UIO_EVENT_MOUSE_MOVE = UIO_EVENT_KEY_DOWN + 2,
	UIO_EVENT_MOUSE_CLICK = UIO_EVENT_KEY_DOWN + 3,
	UIO_EVENT_MOUSE_RELEASE = UIO_EVENT_KEY_DOWN + 4,
};

struct UIO_KeyEvent {
	Uint32 type;
	Uint16 keycode;
};

struct UIO_MouseEvent {
	Uint32 type;
	Uint16 button;
	Sint16 x;
	Sint16 y;
};

union UIO_Event {
	Uint32 type;
	UIO_KeyEvent key;
	UIO_MouseEvent mouse;
};

void allocateUIOEvents();
void uioHookCallback(uiohook_event* const event);
int uioHookThreadFn(void*);

#endif  // UIO_EVENT_HPP_
