#ifndef IMPULSE_CODE_HPP_
#define IMPULSE_CODE_HPP_

#include <SDL3/SDL_stdinc.h>

namespace imp {

using Code      = Uint32;
using TargetTag = Uint32;

struct DeviceAction {
	using T = Sint32;

	static constexpr T KEY_DOWN      = 1;
	static constexpr T KEY_UP        = 2;
	static constexpr T MOUSE_MOVE    = 3;
	static constexpr T MOUSE_CLICK   = 4;
	static constexpr T MOUSE_RELEASE = 5;
	static constexpr T MOUSE_WHEEL   = 6;

	DeviceAction() = delete;
};

struct EventTag {
	using T = Uint32;

	static constexpr T KEY                 = 1;
	static constexpr T MOUSE_BUTTON        = 2;
	static constexpr T MOUSE_MOVE_ABS      = 3;
	static constexpr T MOUSE_MOVE_REL      = 4;
	static constexpr T GAMEPAD_BUTTON      = 5;
	static constexpr T GAMEPAD_TRIGGER     = 6;
	static constexpr T GAMEPAD_STICK_LEFT  = 7;
	static constexpr T GAMEPAD_STICK_RIGHT = 8;
	static constexpr T MOUSE_WHEEL         = 9;

	EventTag() = delete;
};

struct Axis {
	using T = TargetTag;

	static constexpr T X = 1 << 16;
	static constexpr T Y = 2 << 16;

	Axis() = delete;
};

struct Side {
	using T = TargetTag;

	static constexpr T LEFT  = 1 << 16;
	static constexpr T RIGHT = 2 << 16;

	Side() = delete;
};

struct MouseButton {
	using T = TargetTag;

	static constexpr T LEFT   = 1 << 16;
	static constexpr T RIGHT  = 2 << 16;
	static constexpr T MIDDLE = 3 << 16;

	MouseButton() = delete;
};

struct MouseWheel {
	using T = TargetTag;

	static constexpr T UP   = 1 << 16;
	static constexpr T DOWN = 2 << 16;

	MouseWheel() = delete;
};

struct GamepadButton {
	using T = TargetTag;

	static constexpr T NORTH          = 1 << 16;
	static constexpr T SOUTH          = 2 << 16;
	static constexpr T WEST           = 3 << 16;
	static constexpr T EAST           = 4 << 16;
	static constexpr T LEFT_SHOULDER  = 5 << 16;
	static constexpr T RIGHT_SHOULDER = 6 << 16;
	static constexpr T DPAD_UP        = 7 << 16;
	static constexpr T DPAD_DOWN      = 8 << 16;
	static constexpr T DPAD_LEFT      = 9 << 16;
	static constexpr T DPAD_RIGHT     = 10 << 16;
	static constexpr T LEFT_STICK     = 11 << 16;
	static constexpr T RIGHT_STICK    = 12 << 16;

	GamepadButton() = delete;
};

}  // namespace imp

#endif  // IMPULSE_CODE_HPP_
