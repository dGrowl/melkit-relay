#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <libuiohook/uiohook.h>
#include <algorithm>
#include <limits>
#include <ranges>

#include "core/settings.hpp"
#include "math/formula.hpp"
#include "mnk/event.hpp"
#include "vts/input.hpp"
#include "vts/parameter.hpp"
#include "vts/parameter_manager.hpp"
#include "vts/request.hpp"

template <typename T>
T pointerToUnsigned(const void* p) {
	return static_cast<T>(reinterpret_cast<uintptr_t>(p));
}

template <typename T>
T pointerToSigned(const void* p) {
	return static_cast<T>(reinterpret_cast<intptr_t>(p));
}

namespace vts {

void ParameterManager::handleGamepadAxisMotion(SDL_GamepadAxisEvent& event) {
	InputId id = 0;
	switch (event.axis) {
		case SDL_GAMEPAD_AXIS_LEFTX:
			id |= InputEvent::GAMEPAD_STICK_LEFT;
			id |= Axis::X;
			break;
		case SDL_GAMEPAD_AXIS_LEFTY:
			id |= InputEvent::GAMEPAD_STICK_LEFT;
			id |= Axis::Y;
			break;
		case SDL_GAMEPAD_AXIS_RIGHTX:
			id |= InputEvent::GAMEPAD_STICK_RIGHT;
			id |= Axis::X;
			break;
		case SDL_GAMEPAD_AXIS_RIGHTY:
			id |= InputEvent::GAMEPAD_STICK_RIGHT;
			id |= Axis::Y;
			break;
		case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
			id |= InputEvent::GAMEPAD_TRIGGER;
			id |= Side::LEFT;
			break;
		case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
			id |= InputEvent::GAMEPAD_TRIGGER;
			id |= Side::RIGHT;
			break;
	}
	for (auto& parameter : values()) {
		parameter.handleInput(id, event.value);
	}
	_sample.handleInput(id, event.value);
}

void ParameterManager::handleGamepadButton(SDL_GamepadButtonEvent& event,
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
	const float newValue = isClicked ? 1.0f : 0.0f;
	for (auto& parameter : values()) {
		parameter.handleInput(id, newValue);
	}
	_sample.handleInput(id, newValue);
}

void ParameterManager::handleKeyDown(SDL_UserEvent& event) {
	auto         keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id      = InputEvent::KEY | (keycode << 16);
	for (auto& parameter : values()) {
		parameter.handleInput(id, 1.0f);
	}
	_sample.handleInput(id, 1.0f);
}

void ParameterManager::handleKeyUp(SDL_UserEvent& event) {
	auto         keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id      = InputEvent::KEY | (keycode << 16);
	for (auto& parameter : values()) {
		parameter.handleInput(id, 0.0f);
	}
	_sample.handleInput(id, 0.0f);
}

void ParameterManager::handleMouseButton(SDL_UserEvent& event, bool isClicked) {
	auto         button   = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id       = InputEvent::MOUSE_BUTTON | button;
	const float  newValue = isClicked ? 1.0f : 0.0f;
	for (auto& parameter : values()) {
		parameter.handleInput(id, newValue);
	}
	_sample.handleInput(id, newValue);
}

constexpr InputId MOUSE_MOVE_ABS_X = InputEvent::MOUSE_MOVE_ABS | Axis::X;
constexpr InputId MOUSE_MOVE_ABS_Y = InputEvent::MOUSE_MOVE_ABS | Axis::Y;
constexpr InputId MOUSE_MOVE_REL_X = InputEvent::MOUSE_MOVE_REL | Axis::X;
constexpr InputId MOUSE_MOVE_REL_Y = InputEvent::MOUSE_MOVE_REL | Axis::Y;

void ParameterManager::handleMouseMove(SDL_UserEvent& event) {
	auto x = pointerToSigned<Sint16>(event.data1);
	auto y = pointerToSigned<Sint16>(event.data2);
	_mouse.dx += (x - _mouse.x) * _mouseCoefficient;
	_mouse.dy += (y - _mouse.y) * _mouseCoefficient;
	_mouse.x = x;
	_mouse.y = y;
}

float calcMouseCoefficient(const int sensitivity) {
	return math::remapLinear<float>(sensitivity, 1.0f, 100.0f, 0.01f, 4.0f);
}

ParameterManager::ParameterManager() :
    _mouse(),
    _sample(),
    _params(),
    _mouseCoefficient(1.0f),
    _mouseSensitivity(SETTINGS.getMouseSensitivity()),
    _lastUpdateTimeMs(0) {
	_mouseCoefficient = calcMouseCoefficient(_mouseSensitivity);
}

Parameter& ParameterManager::operator[](const char* name) {
	return _params[name];
}

int ParameterManager::getMouseSensitivity() const {
	return _mouseSensitivity;
}

std::unordered_map<std::string, Parameter>::iterator ParameterManager::end() {
	return _params.end();
}

std::unordered_map<std::string, Parameter>::iterator ParameterManager::find(
    const std::string& name) {
	return _params.find(name);
}

Parameter& ParameterManager::getSample() {
	return _sample;
}

auto ParameterManager::values() -> ParameterView {
	return _params | std::views::values;
}

void ParameterManager::add(const std::string& name) {
	_params.emplace(name, name);
}

void ParameterManager::clear() {
	_params.clear();
}

void ParameterManager::handleGamepadEvent(
    SDL_Event&           event,
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

void ParameterManager::handleEvent(SDL_UserEvent& event) {
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
	}
}

void ParameterManager::setMouseSensitivity(const int sensitivity) {
	if (sensitivity == _mouseSensitivity) {
		return;
	}
	SETTINGS.setMouseSensitivity(sensitivity);
	_mouseSensitivity = sensitivity;
	_mouseCoefficient = calcMouseCoefficient(sensitivity);
}

constexpr float sign(const float x) {
	if (x > 0.0f)
		return 1.0f;
	if (x < 0.0f)
		return -1.0f;
	return 0.0f;
}

constexpr float MOUSE_DELTA_DECAY_RATE_MS = .65f;

constexpr float MAX_MOUSE_DELTA = 64.0f;

void ParameterManager::update() {
	const Uint64 timeMs = SDL_GetTicks();
	if (_mouse.dx == 0.0f && _mouse.dy == 0.0f) {
		return;
	}
	const Uint64 dtMs  = timeMs - _lastUpdateTimeMs;
	const float  decay = MOUSE_DELTA_DECAY_RATE_MS * dtMs;

	_mouse.dx = sign(_mouse.dx)
	            * std::clamp(std::abs(_mouse.dx) - decay, 0.0f, MAX_MOUSE_DELTA);
	_mouse.dy = sign(_mouse.dy)
	            * std::clamp(std::abs(_mouse.dy) - decay, 0.0f, MAX_MOUSE_DELTA);

	for (auto& parameter : values()) {
		parameter.handleInput(MOUSE_MOVE_ABS_X, _mouse.x);
		parameter.handleInput(MOUSE_MOVE_ABS_Y, _mouse.y);
		parameter.handleInput(MOUSE_MOVE_REL_X, _mouse.dx);
		parameter.handleInput(MOUSE_MOVE_REL_Y, _mouse.dy);
	}
	_sample.handleInput(MOUSE_MOVE_ABS_X, _mouse.x);
	_sample.handleInput(MOUSE_MOVE_ABS_Y, _mouse.y);
	_sample.handleInput(MOUSE_MOVE_REL_X, _mouse.dx);
	_sample.handleInput(MOUSE_MOVE_REL_Y, _mouse.dy);
	_lastUpdateTimeMs = timeMs;
}

}  // namespace vts
