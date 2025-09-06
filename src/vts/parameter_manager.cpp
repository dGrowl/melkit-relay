#include <algorithm>
#include <ranges>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <libuiohook/uiohook.h>

#include "mnk/event.hpp"
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

void ParameterManager::handleKeyDown(SDL_UserEvent& event) {
	auto keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id = InputEvent::KEY | (keycode << 16);
	for (auto& parameter : values()) {
		parameter.handleInput(id, 1.0f);
	}
	_sample.handleInput(id, 1.0f);
}

void ParameterManager::handleKeyUp(SDL_UserEvent& event) {
	auto keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id = InputEvent::KEY | (keycode << 16);
	for (auto& parameter : values()) {
		parameter.handleInput(id, 0.0f);
	}
	_sample.handleInput(id, 0.0f);
}

void ParameterManager::handleMouseButton(SDL_UserEvent& event, bool isClicked) {
	auto button = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id = InputEvent::MOUSE_BUTTON | button;
	const float newValue = isClicked ? 1.0f : 0.0f;
	for (auto& parameter : values()) {
		parameter.handleInput(id, newValue);
	}
	_sample.handleInput(id, newValue);
}

Parameter& ParameterManager::operator[](const char* name) {
	return _params[name];
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

constexpr InputId MOUSE_MOVE_ABS_X =
    InputEvent::MOUSE_MOVE_ABS | static_cast<InputId>(Axis::X);

constexpr InputId MOUSE_MOVE_ABS_Y =
    InputEvent::MOUSE_MOVE_ABS | static_cast<InputId>(Axis::Y);

constexpr InputId MOUSE_MOVE_REL_X =
    InputEvent::MOUSE_MOVE_REL | static_cast<InputId>(Axis::X);

constexpr InputId MOUSE_MOVE_REL_Y =
    InputEvent::MOUSE_MOVE_REL | static_cast<InputId>(Axis::Y);

void ParameterManager::handleMouseMove(SDL_UserEvent& event) {
	auto x = pointerToSigned<Sint16>(event.data1);
	auto y = pointerToSigned<Sint16>(event.data2);
	_mouse.dx += x - _mouse.x;
	_mouse.dy += y - _mouse.y;
	_mouse.x = x;
	_mouse.y = y;
}

ParameterManager::ParameterManager() :
    _mouse(),
    _sample(),
    _params(),
    _lastUpdateTimeMs(0),
    _nextUpdateTimeMs(0) {}

void ParameterManager::add(const ParameterData& data) {
	_params.emplace(data.name, data);
}

void ParameterManager::clear() {
	_params.clear();
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

constexpr float sign(const float x) {
	if (x > 0.0f)
		return 1.0f;
	if (x < 0.0f)
		return -1.0f;
	return 0.0f;
}

constexpr Uint64 UPDATE_DELAY_MS = 20;

constexpr float MOUSE_DELTA_DECAY_RATE_MS = .65f;

constexpr float MOUSE_DELTA_MAX = 64.0f;

void ParameterManager::update() {
	const Uint64 timeMs = SDL_GetTicks();
	if (timeMs <= _nextUpdateTimeMs) {
		return;
	}
	if (_mouse.dx == 0.0f && _mouse.dy == 0.0f) {
		return;
	}
	const Uint64 dtMs = timeMs - _lastUpdateTimeMs;
	const float decay = MOUSE_DELTA_DECAY_RATE_MS * dtMs;
	_mouse.dx = sign(_mouse.dx)
	            * std::clamp(std::abs(_mouse.dx) - decay, 0.0f, MOUSE_DELTA_MAX);
	_mouse.dy = sign(_mouse.dy)
	            * std::clamp(std::abs(_mouse.dy) - decay, 0.0f, MOUSE_DELTA_MAX);
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
	_nextUpdateTimeMs = timeMs + UPDATE_DELAY_MS;
	_lastUpdateTimeMs = timeMs;
}

}  // namespace vts
