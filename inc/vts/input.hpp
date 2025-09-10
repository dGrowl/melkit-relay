#ifndef VTS_INPUT_HPP_
#define VTS_INPUT_HPP_
#include <SDL3/SDL_gamepad.h>
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
	MOUSE_MOVE_REL,
	GAMEPAD_BUTTON,
	GAMEPAD_TRIGGER,
	GAMEPAD_STICK_LEFT,
	GAMEPAD_STICK_RIGHT
};

enum Axis : Uint32 {
	X = 1 << 16,
	Y = 2 << 16,
};

struct Side {
	static constexpr Uint32 LEFT  = 1 << 16;
	static constexpr Uint32 RIGHT = 2 << 16;
};

enum MouseButton : Uint32 {
	LEFT   = 1 << 16,
	RIGHT  = 2 << 16,
	MIDDLE = 3 << 16,
	FOURTH = 4 << 16,
	FIFTH  = 5 << 16,
};

enum GamepadButton : Uint32 {
	NORTH          = 1 << 16,
	SOUTH          = 2 << 16,
	WEST           = 3 << 16,
	EAST           = 4 << 16,
	LEFT_SHOULDER  = 5 << 16,
	RIGHT_SHOULDER = 6 << 16,
	DPAD_UP        = 7 << 16,
	DPAD_DOWN      = 8 << 16,
	DPAD_LEFT      = 9 << 16,
	DPAD_RIGHT     = 10 << 16,
	LEFT_STICK     = 11 << 16,
	RIGHT_STICK    = 12 << 16,
};

class InputData {
private:
	InputId _id;

	float _inMax;
	float _inMin;
	float _outMax;
	float _outMin;
	float _value;

public:
	bool isInverted;

	InputData(const InputId id);

	float   getMax() const;
	float   getMin() const;
	float   getValue() const;
	InputId getId() const;
	void    update(float inValue);
};

}  // namespace vts

#endif  // VTS_INPUT_HPP_
