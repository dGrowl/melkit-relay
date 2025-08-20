#ifndef MNK_EVENT_HPP_
#define MNK_EVENT_HPP_

#include <SDL3/SDL_events.h>

namespace mnk {

constexpr int N_EVENTS = 2;

enum Event : Uint32 {
	START = SDL_EVENT_USER,
	INPUT = START + 1
};

enum InputCode : Sint32 {
	KEY_DOWN,
	KEY_UP,
	MOUSE_MOVE,
	MOUSE_CLICK,
	MOUSE_RELEASE,
};

struct MouseData {
	Uint16 button;
	Sint16 x;
	Sint16 y;
};

struct KeyboardData {
	Uint16 keycode;
};

void allocateEvents();

}  // namespace mnk

#endif  // MNK_EVENT_HPP_
