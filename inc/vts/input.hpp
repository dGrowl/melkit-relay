#ifndef VTS_INPUT_HPP_
#define VTS_INPUT_HPP_

#include <SDL3/SDL_stdinc.h>

namespace vts {

enum ActionCode : Sint32 {
	KEY_DOWN,
	KEY_UP,
	MOUSE_MOVE,
	MOUSE_CLICK,
	MOUSE_RELEASE
};

enum InputType : Uint32 {
	KEY,
	MOUSE_BUTTON,
	MOUSE_MOVE_ABS,
	MOUSE_MOVE_REL
};

enum Axis : Uint32 {
	X = 1 << 16,
	Y,
};

enum Button : Uint32 {
	LEFT = 1 << 16,
	RIGHT,
	MIDDLE,
	FOURTH,
	FIFTH
};

}  // namespace vts

#endif  // VTS_INPUT_HPP_
