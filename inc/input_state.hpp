#ifndef INPUT_STATE_HPP_
#define INPUT_STATE_HPP_

#include <unordered_set>

#include "uio_event.hpp"

struct MouseState {
	int x = 0;
	int y = 0;
	int dx = 0;
	int dy = 0;
	bool left = false;
	bool middle = false;
	bool right = false;
	bool fourth = false;
	bool fifth = false;
};

struct KeyboardState {
	std::unordered_set<int> held;
};

class InputState {
private:
	MouseState _mouse;
	KeyboardState _key;

public:
	InputState() = default;

	void handleKeyDown(SDL_Event& event);
	void handleKeyUp(SDL_Event& event);
	void handleMouseButton(SDL_Event& event, bool isClicked);
	void handleMouseMove(SDL_Event& event);
};

#endif  // INPUT_STATE_HPP_
