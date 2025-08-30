#ifndef VTS_PARAMETER_MANAGER_HPP_
#define VTS_PARAMETER_MANAGER_HPP_

#include <ranges>
#include <string>
#include <unordered_map>

#include <SDL3/SDL_events.h>

#include "vts/input.hpp"
#include "vts/parameter.hpp"

namespace vts {

struct MouseState {
	int x = 0;
	int y = 0;
};

using ParameterView =
    decltype(std::declval<std::unordered_map<std::string, Parameter>&>()
             | std::views::values);

class ParameterManager {
private:
	MouseState _mouse;
	Parameter _sample;
	std::unordered_map<std::string, Parameter> _params;

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);

public:
	ParameterManager() = default;
	Parameter& operator[](const char* name);
	Parameter& getSample();
	ParameterView values();
	void add(const ParameterData& data);
	void clear();
	void handleEvent(SDL_UserEvent& event);
};

}  // namespace vts

#endif  // VTS_PARAMETER_MANAGER_HPP_
