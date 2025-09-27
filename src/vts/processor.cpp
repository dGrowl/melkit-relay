#include <algorithm>
#include <limits>
#include <ranges>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <libuiohook/uiohook.h>

#include "core/settings.hpp"
#include "math/formula.hpp"
#include "math/geometry.hpp"
#include "mnk/event.hpp"
#include "vts/input.hpp"
#include "vts/processor.hpp"

template <typename T>
T pointerToUnsigned(const void* p) {
	return static_cast<T>(reinterpret_cast<uintptr_t>(p));
}

template <typename T>
T pointerToSigned(const void* p) {
	return static_cast<T>(reinterpret_cast<intptr_t>(p));
}

static constexpr float DEADZONE   = 3000.0f;
static constexpr float SATURATION = 3000.0f;
static constexpr float STICK_LOWER =
    std::numeric_limits<Sint16>::min() + SATURATION;
static constexpr float STICK_UPPER =
    std::numeric_limits<Sint16>::max() - SATURATION;

static constexpr float transformStick(float x,
                                      float outLower = -1.0f,
                                      float outUpper = 1.0f) {
	x = std::clamp(x, STICK_LOWER, STICK_UPPER);
	x = math::remapLinearDeadzone(x,
	                              STICK_LOWER,
	                              STICK_UPPER,
	                              outLower,
	                              outUpper,
	                              DEADZONE);
	return x;
}

static constexpr float transformTrigger(float x) {
	x = std::clamp(x, DEADZONE, STICK_UPPER);
	x = math::remapLinear(x, DEADZONE, STICK_UPPER, 0.0f, 1.0f);
	return x;
}

static constexpr float transformMousePosition(float x,
                                              float inLower,
                                              float inUpper,
                                              float outLower = 0.0f,
                                              float outUpper = 1.0f) {
	x = std::clamp(x, inLower, inUpper);
	x = math::remapLinear(x, inLower, inUpper, outLower, outUpper);
	return x;
}

static constexpr float MAX_MOUSE_MOTION_DELTA = 64.0f;

static constexpr float transformMouseDelta(float x,
                                           float outLower = -1.0f,
                                           float outUpper = 1.0f) {
	x = std::clamp(x, -MAX_MOUSE_MOTION_DELTA, MAX_MOUSE_MOTION_DELTA);
	x = math::remapLinear(x,
	                      -MAX_MOUSE_MOTION_DELTA,
	                      MAX_MOUSE_MOTION_DELTA,
	                      outLower,
	                      outUpper);
	return x;
}

static constexpr float MAX_MOUSE_WHEEL_DELTA = 64.0f;

static constexpr float transformMouseWheel(float x) {
	x = std::clamp(x, 0.0f, MAX_MOUSE_WHEEL_DELTA);
	x = math::remapLinear(x, 0.0f, MAX_MOUSE_WHEEL_DELTA, 0.0f, 1.0f);
	return x;
}

namespace vts {

void Processor::handleGamepadAxisMotion(SDL_GamepadAxisEvent& event) {
	InputId id    = 0;
	float   value = event.value;
	switch (event.axis) {
		case SDL_GAMEPAD_AXIS_LEFTX:
			id |= InputEvent::GAMEPAD_STICK_LEFT;
			id |= Axis::X;
			value = transformStick(value);
			break;
		case SDL_GAMEPAD_AXIS_LEFTY:
			id |= InputEvent::GAMEPAD_STICK_LEFT;
			id |= Axis::Y;
			value = transformStick(value, 1.0f, -1.0f);
			break;
		case SDL_GAMEPAD_AXIS_RIGHTX:
			id |= InputEvent::GAMEPAD_STICK_RIGHT;
			id |= Axis::X;
			value = transformStick(value);
			break;
		case SDL_GAMEPAD_AXIS_RIGHTY:
			id |= InputEvent::GAMEPAD_STICK_RIGHT;
			id |= Axis::Y;
			value = transformStick(value, 1.0f, -1.0f);
			break;
		case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
			id |= InputEvent::GAMEPAD_TRIGGER;
			id |= Side::LEFT;
			value = transformTrigger(value);
			break;
		case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
			id |= InputEvent::GAMEPAD_TRIGGER;
			id |= Side::RIGHT;
			value = transformTrigger(value);
			break;
	}
	_queue.emplace_back(id, value);
}

void Processor::handleGamepadButton(SDL_GamepadButtonEvent& event,
                                    const bool              isClicked) {
	InputId id = InputEvent::GAMEPAD_BUTTON;
	switch (event.button) {
		case SDL_GAMEPAD_BUTTON_NORTH:
			id |= GamepadButton::NORTH;
			break;
		case SDL_GAMEPAD_BUTTON_SOUTH:
			id |= GamepadButton::SOUTH;
			break;
		case SDL_GAMEPAD_BUTTON_WEST:
			id |= GamepadButton::WEST;
			break;
		case SDL_GAMEPAD_BUTTON_EAST:
			id |= GamepadButton::EAST;
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_SHOULDER:
			id |= GamepadButton::LEFT_SHOULDER;
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER:
			id |= GamepadButton::RIGHT_SHOULDER;
			break;
		case SDL_GAMEPAD_BUTTON_DPAD_UP:
			id |= GamepadButton::DPAD_UP;
			break;
		case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
			id |= GamepadButton::DPAD_DOWN;
			break;
		case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
			id |= GamepadButton::DPAD_LEFT;
			break;
		case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
			id |= GamepadButton::DPAD_RIGHT;
			break;
		case SDL_GAMEPAD_BUTTON_LEFT_STICK:
			id |= GamepadButton::LEFT_STICK;
			break;
		case SDL_GAMEPAD_BUTTON_RIGHT_STICK:
			id |= GamepadButton::RIGHT_STICK;
			break;
	}
	_queue.emplace_back(id, isClicked ? 1.0f : 0.0f);
}

void Processor::handleKeyDown(SDL_UserEvent& event) {
	auto         keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id      = InputEvent::KEY | (keycode << 16);
	_queue.emplace_back(id, 1.0f);
}

void Processor::handleKeyUp(SDL_UserEvent& event) {
	auto         keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id      = InputEvent::KEY | (keycode << 16);
	_queue.emplace_back(id, 0.0f);
}

void Processor::handleMouseButton(SDL_UserEvent& event, bool isClicked) {
	auto         button = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id     = InputEvent::MOUSE_BUTTON | button;
	_queue.emplace_back(id, isClicked ? 1.0f : 0.0f);
}

void Processor::handleMouseMove(SDL_UserEvent& event) {
	int x = pointerToSigned<Sint16>(event.data1);
	int y = pointerToSigned<Sint16>(event.data2);
	_mouseState.dx += (x - _mouseState.x) * _mouseCoefficient;
	_mouseState.dy += (y - _mouseState.y) * _mouseCoefficient;
	_mouseState.x = x;
	_mouseState.y = y;
}

void Processor::handleMouseWheel(SDL_UserEvent& event) {
	auto rotation = pointerToSigned<Sint16>(event.data1);
	if (rotation == -1) {
		_mouseState.wheelUp += MAX_MOUSE_WHEEL_DELTA * 2.0f;
	}
	else {
		_mouseState.wheelDown += MAX_MOUSE_WHEEL_DELTA * 2.0f;
	}
}

static constexpr float   MOUSE_DECAY_RATE_PER_MS = .65f;
static constexpr InputId MOUSE_MOVE_ABS_X =
    InputEvent::MOUSE_MOVE_ABS | Axis::X;
static constexpr InputId MOUSE_MOVE_ABS_Y =
    InputEvent::MOUSE_MOVE_ABS | Axis::Y;
static constexpr InputId MOUSE_MOVE_REL_X =
    InputEvent::MOUSE_MOVE_REL | Axis::X;
static constexpr InputId MOUSE_MOVE_REL_Y =
    InputEvent::MOUSE_MOVE_REL | Axis::Y;

void Processor::updateMouseMovement(const Uint64 dtMs) {
	if (_mouseState.dx == 0.0f && _mouseState.dy == 0.0f) {
		return;
	}
	const float decay = MOUSE_DECAY_RATE_PER_MS * dtMs;

	float x        = transformMousePosition(_mouseState.x,
                                  _mouseBounds.left,
                                  _mouseBounds.right);
	float y        = transformMousePosition(_mouseState.y,
                                  _mouseBounds.top,
                                  _mouseBounds.bottom,
                                  1.0f,
                                  0.0f);
	_mouseState.dx = math::sign(_mouseState.dx)
	                 * std::clamp(std::abs(_mouseState.dx) - decay,
	                              0.0f,
	                              MAX_MOUSE_MOTION_DELTA);
	_mouseState.dy = math::sign(_mouseState.dy)
	                 * std::clamp(std::abs(_mouseState.dy) - decay,
	                              0.0f,
	                              MAX_MOUSE_MOTION_DELTA);

	float dx = transformMouseDelta(_mouseState.dx);
	float dy = transformMouseDelta(_mouseState.dy, 1.0f, -1.0f);

	_queue.emplace_back(MOUSE_MOVE_ABS_X, x);
	_queue.emplace_back(MOUSE_MOVE_ABS_Y, y);
	_queue.emplace_back(MOUSE_MOVE_REL_X, dx);
	_queue.emplace_back(MOUSE_MOVE_REL_Y, dy);
}

static constexpr float   MOUSE_WHEEL_DECAY_RATE_PER_MS = .35f;
static constexpr InputId MOUSE_WHEEL_UP =
    InputEvent::MOUSE_WHEEL | MouseWheel::UP;
static constexpr InputId MOUSE_WHEEL_DOWN =
    InputEvent::MOUSE_WHEEL | MouseWheel::DOWN;

void Processor::updateMouseWheel(const Uint64 dtMs) {
	if (_mouseState.wheelUp == 0.0f && _mouseState.wheelDown == 0.0f) {
		return;
	}
	const float decay = MOUSE_WHEEL_DECAY_RATE_PER_MS * dtMs;

	_mouseState.wheelUp =
	    std::clamp(_mouseState.wheelUp - decay, 0.0f, MAX_MOUSE_WHEEL_DELTA * 2);
	_mouseState.wheelDown =
	    std::clamp(_mouseState.wheelDown - decay, 0.0f, MAX_MOUSE_WHEEL_DELTA * 2);

	float wheelUp   = transformMouseWheel(_mouseState.wheelUp);
	float wheelDown = transformMouseWheel(_mouseState.wheelDown);

	_queue.emplace_back(MOUSE_WHEEL_UP, wheelUp);
	_queue.emplace_back(MOUSE_WHEEL_DOWN, wheelDown);
}

static constexpr float MIN_MOUSE_COEFFICIENT = 0.0000001f;
static constexpr float MAX_MOUSE_COEFFICIENT = 4.0f;

static constexpr float calcMouseCoefficient(const int sensitivity) {
	return math::remapLinear<float>(sensitivity,
	                                1.0f,
	                                100.0f,
	                                MIN_MOUSE_COEFFICIENT,
	                                MAX_MOUSE_COEFFICIENT);
}

static constexpr size_t MAX_EXPECTED_IMPULSES = 16;

Processor::Processor() :
    _mouseCoefficient(1.0f),
    _mouseSensitivity(SETTINGS.getMouseSensitivity()),
    _mouseBounds(SETTINGS.getMouseBounds()),
    _mouseState(),
    _lastUpdateTimeMs(0),
    _queue() {
	_mouseCoefficient = calcMouseCoefficient(_mouseSensitivity);
	_queue.reserve(MAX_EXPECTED_IMPULSES);
}

const ImpulseQueue& Processor::impulses() const {
	return _queue;
}

const math::Rectangle<int>& Processor::getMouseBounds() const {
	return _mouseBounds;
}

const MouseState& Processor::getMouseState() const {
	return _mouseState;
}

int Processor::getMouseSensitivity() const {
	return _mouseSensitivity;
}

void Processor::clear() {
	_queue.clear();
}

void Processor::handleGamepadEvent(SDL_Event&           event,
                                   const SDL_JoystickID activeGamepadId) {
	switch (event.type) {
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if (event.gbutton.which != activeGamepadId) {
				return;
			}
			handleGamepadButton(event.gbutton, true);
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_UP:
			if (event.gbutton.which != activeGamepadId) {
				return;
			}
			handleGamepadButton(event.gbutton, false);
			break;
		case SDL_EVENT_GAMEPAD_AXIS_MOTION:
			if (event.gaxis.which != activeGamepadId) {
				return;
			}
			handleGamepadAxisMotion(event.gaxis);
			break;
	}
}

void Processor::handleEvent(SDL_UserEvent& event) {
	switch (event.code) {
		case vts::ActionCode::KEY_DOWN:
			handleKeyDown(event);
			break;
		case vts::ActionCode::KEY_UP:
			handleKeyUp(event);
			break;
		case vts::ActionCode::MOUSE_MOVE:
			handleMouseMove(event);
			break;
		case vts::ActionCode::MOUSE_CLICK:
			handleMouseButton(event, true);
			break;
		case vts::ActionCode::MOUSE_RELEASE:
			handleMouseButton(event, false);
			break;
		case vts::ActionCode::MOUSE_WHEEL:
			handleMouseWheel(event);
			break;
	}
}

void Processor::setMouseBounds(const math::Rectangle<int>& bounds) {
	if (bounds.top >= bounds.bottom || bounds.left >= bounds.right) {
		return;
	}
	SETTINGS.setMouseBounds(bounds);
	_mouseBounds = bounds;
}

static constexpr int MOUSE_MIN_SENSITIVITY = 1;
static constexpr int MOUSE_MAX_SENSITIVITY = 100;

void Processor::setMouseSensitivity(const int sensitivity) {
	if (sensitivity == _mouseSensitivity) {
		return;
	}
	if ((sensitivity < MOUSE_MIN_SENSITIVITY)
	    || (sensitivity > MOUSE_MAX_SENSITIVITY)) {
		return;
	}
	SETTINGS.setMouseSensitivity(sensitivity);
	_mouseSensitivity = sensitivity;
	_mouseCoefficient = calcMouseCoefficient(sensitivity);
}

void Processor::update() {
	const Uint64 timeMs = SDL_GetTicks();
	const Uint64 dtMs   = timeMs - _lastUpdateTimeMs;
	updateMouseMovement(dtMs);
	updateMouseWheel(dtMs);
	_lastUpdateTimeMs = timeMs;
}

}  // namespace vts
