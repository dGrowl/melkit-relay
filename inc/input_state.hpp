#ifndef INPUT_STATE_HPP_
#define INPUT_STATE_HPP_

#include <unordered_set>

#include <SDL3/SDL_events.h>

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

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);

public:
	InputState() = default;
	void handleEvent(SDL_UserEvent& event);
};

#endif  // INPUT_STATE_HPP_
