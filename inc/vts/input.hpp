#ifndef VTS_INPUT_HPP_
#define VTS_INPUT_HPP_

#include <SDL3/SDL_stdinc.h>

namespace vts {

using InputId = Uint32;

enum ActionCode : Sint32 {
	KEY_DOWN = 1,
	KEY_UP,
	MOUSE_MOVE,
	MOUSE_CLICK,
	MOUSE_RELEASE
};

enum InputEvent : Uint32 {
	KEY = 1,
	MOUSE_BUTTON,
	MOUSE_MOVE_ABS,
	MOUSE_MOVE_REL
};

enum Axis : Uint32 {
	X = 1 << 16,
	Y = 2 << 16,
};

enum Button : Uint32 {
	LEFT = 1 << 16,
	RIGHT = 2 << 16,
	MIDDLE = 3 << 16,
	FOURTH = 4 << 16,
	FIFTH = 5 << 16,
};

class InputData {
private:
	InputId _id;

public:
	float min;
	float max;
	float value;

	InputData(const InputId id);

	inline InputId getId() const { return _id; }
};

}  // namespace vts

#endif  // VTS_INPUT_HPP_
