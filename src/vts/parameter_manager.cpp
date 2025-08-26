#include <SDL3/SDL_events.h>
#include <libuiohook/uiohook.h>

#include "mnk/event.hpp"
#include "vts/parameter.hpp"
#include "vts/parameter_manager.hpp"

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
	const Uint32 id = InputType::KEY | (keycode << 16);
	for (auto& pair : _params) {
		pair.second.handleInput(id, 1.0f);
	}
}

void ParameterManager::handleKeyUp(SDL_UserEvent& event) {
	auto keycode = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id = InputType::KEY | (keycode << 16);
	for (auto& pair : _params) {
		pair.second.handleInput(id, 0.0f);
	}
}

void ParameterManager::handleMouseButton(SDL_UserEvent& event, bool isClicked) {
	auto button = pointerToUnsigned<Uint32>(event.data1);
	const Uint32 id = InputType::MOUSE_BUTTON | button;
	for (auto& pair : _params) {
		pair.second.handleInput(id, isClicked ? 1.0f : 0.0f);
	}
}

Parameter& ParameterManager::operator[](const char* name) {
	return _params[name];
}

auto ParameterManager::values() -> ParameterView {
	return _params | std::views::values;
}

constexpr Uint32 MOUSE_MOVE_ABS_X =
    InputType::MOUSE_MOVE_ABS | static_cast<Uint32>(Axis::X);

constexpr Uint32 MOUSE_MOVE_ABS_Y =
    InputType::MOUSE_MOVE_ABS | static_cast<Uint32>(Axis::Y);

constexpr Uint32 MOUSE_MOVE_REL_X =
    InputType::MOUSE_MOVE_REL | static_cast<Uint32>(Axis::X);

constexpr Uint32 MOUSE_MOVE_REL_Y =
    InputType::MOUSE_MOVE_REL | static_cast<Uint32>(Axis::Y);

void ParameterManager::handleMouseMove(SDL_UserEvent& event) {
	auto x = pointerToSigned<Sint16>(event.data1);
	auto y = pointerToSigned<Sint16>(event.data2);
	auto dx = x - _mouse.x;
	auto dy = y - _mouse.y;
	_mouse.x = x;
	_mouse.y = y;
	for (auto& pair : _params) {
		pair.second.handleInput(MOUSE_MOVE_ABS_X, x);
		pair.second.handleInput(MOUSE_MOVE_ABS_Y, y);
		pair.second.handleInput(MOUSE_MOVE_REL_X, dx);
		pair.second.handleInput(MOUSE_MOVE_REL_Y, dy);
	}
}

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

}  // namespace vts
