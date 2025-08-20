#include <libuiohook/uiohook.h>
#include <SDL3/SDL_log.h>

#include "input_state.hpp"
#include "mnk/event.hpp"

void InputState::handleKeyDown(SDL_UserEvent& event) {
	auto* data = static_cast<mnk::KeyboardData*>(event.data1);
	_key.held.insert(data->keycode);
	delete data;
}

void InputState::handleKeyUp(SDL_UserEvent& event) {
	auto* data = static_cast<mnk::KeyboardData*>(event.data1);
	_key.held.erase(data->keycode);
	delete data;
}

void InputState::handleMouseButton(SDL_UserEvent& event, bool isClicked) {
	auto* data = static_cast<mnk::MouseData*>(event.data1);
	switch (data->button) {
		case MOUSE_BUTTON1:
			_mouse.left = isClicked;
			break;
		case MOUSE_BUTTON2:
			_mouse.right = isClicked;
			break;
		case MOUSE_BUTTON3:
			_mouse.middle = isClicked;
			break;
		case MOUSE_BUTTON4:
			_mouse.fourth = isClicked;
			break;
		case MOUSE_BUTTON5:
			_mouse.fifth = isClicked;
			break;
	}
	delete data;
}

void InputState::handleMouseMove(SDL_UserEvent& event) {
	auto* data = static_cast<mnk::MouseData*>(event.data1);
	Sint16 x = data->x;
	Sint16 y = data->y;
	_mouse.dx = x - _mouse.x;
	_mouse.dy = y - _mouse.y;
	_mouse.x = x;
	_mouse.y = y;
	delete data;
}

void InputState::handleEvent(SDL_UserEvent& event) {
	switch (event.code) {
		case mnk::InputCode::KEY_DOWN:
			handleKeyDown(event);
			break;
		case mnk::InputCode::KEY_UP:
			handleKeyUp(event);
			break;
		case mnk::InputCode::MOUSE_MOVE:
			handleMouseMove(event);
			break;
		case mnk::InputCode::MOUSE_CLICK:
			handleMouseButton(event, true);
			break;
		case mnk::InputCode::MOUSE_RELEASE:
			handleMouseButton(event, false);
			break;
	}
}
