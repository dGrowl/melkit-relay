#ifndef IMPULSE_INPUT_HPP_
#define IMPULSE_INPUT_HPP_

#include <SDL3/SDL_stdinc.h>

namespace imp {

using InputId   = Uint32;
using EventTag  = Uint32;
using TargetTag = Uint32;

enum ActionCode : Sint32 {
	KEY_DOWN = 1,
	KEY_UP,
	MOUSE_MOVE,
	MOUSE_CLICK,
	MOUSE_RELEASE,
	MOUSE_WHEEL
};

struct InputEvent {
	static constexpr EventTag KEY                 = 1;
	static constexpr EventTag MOUSE_BUTTON        = 2;
	static constexpr EventTag MOUSE_MOVE_ABS      = 3;
	static constexpr EventTag MOUSE_MOVE_REL      = 4;
	static constexpr EventTag GAMEPAD_BUTTON      = 5;
	static constexpr EventTag GAMEPAD_TRIGGER     = 6;
	static constexpr EventTag GAMEPAD_STICK_LEFT  = 7;
	static constexpr EventTag GAMEPAD_STICK_RIGHT = 8;
	static constexpr EventTag MOUSE_WHEEL         = 9;
};

struct Axis {
	static constexpr TargetTag X = 1 << 16;
	static constexpr TargetTag Y = 2 << 16;
};

struct Side {
	static constexpr TargetTag LEFT  = 1 << 16;
	static constexpr TargetTag RIGHT = 2 << 16;
};

struct MouseButton {
	static constexpr TargetTag LEFT   = 1 << 16;
	static constexpr TargetTag RIGHT  = 2 << 16;
	static constexpr TargetTag MIDDLE = 3 << 16;
};

struct MouseWheel {
	static constexpr TargetTag UP   = 1 << 16;
	static constexpr TargetTag DOWN = 2 << 16;
};

struct GamepadButton {
	static constexpr TargetTag NORTH          = 1 << 16;
	static constexpr TargetTag SOUTH          = 2 << 16;
	static constexpr TargetTag WEST           = 3 << 16;
	static constexpr TargetTag EAST           = 4 << 16;
	static constexpr TargetTag LEFT_SHOULDER  = 5 << 16;
	static constexpr TargetTag RIGHT_SHOULDER = 6 << 16;
	static constexpr TargetTag DPAD_UP        = 7 << 16;
	static constexpr TargetTag DPAD_DOWN      = 8 << 16;
	static constexpr TargetTag DPAD_LEFT      = 9 << 16;
	static constexpr TargetTag DPAD_RIGHT     = 10 << 16;
	static constexpr TargetTag LEFT_STICK     = 11 << 16;
	static constexpr TargetTag RIGHT_STICK    = 12 << 16;
};

class InputData {
private:
	InputId _id;

	bool  _isInverted;
	float _inMax;
	float _inMin;
	float _outMax;
	float _outMin;
	float _value;

public:
	InputData(const InputId id, const bool isInverted = false);

	bool    getIsInverted() const;
	bool&   isInvertedRef();
	float   getMax() const;
	float   getMin() const;
	float   getValue() const;
	InputId getId() const;
	void    update(float inValue);
};

}  // namespace imp

#endif  // IMPULSE_INPUT_HPP_
