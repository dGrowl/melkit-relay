#include "input_state.hpp"

void InputState::handleKeyDown(SDL_Event& event) {
	auto* e = static_cast<UIO_Event*>(event.user.data1);
	_key.held.insert(e->key.keycode);
	SDL_free(e);
}

void InputState::handleKeyUp(SDL_Event& event) {
	auto* e = static_cast<UIO_Event*>(event.user.data1);
	_key.held.erase(e->key.keycode);
	SDL_free(e);
}

void InputState::handleMouseButton(SDL_Event& event, bool isClicked) {
	auto* e = static_cast<UIO_Event*>(event.user.data1);
	switch (e->mouse.button) {
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
	SDL_free(e);
}

void InputState::handleMouseMove(SDL_Event& event) {
	auto* e = static_cast<UIO_Event*>(event.user.data1);
	Sint16 x = e->mouse.x;
	Sint16 y = e->mouse.y;
	_mouse.dx = x - _mouse.x;
	_mouse.dy = y - _mouse.y;
	_mouse.x = x;
	_mouse.y = y;
	SDL_free(e);
}
