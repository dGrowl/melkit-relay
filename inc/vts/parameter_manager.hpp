#ifndef VTS_PARAMETER_MANAGER_HPP_
#define VTS_PARAMETER_MANAGER_HPP_

#include <ranges>
#include <string>
#include <unordered_map>

#include "vts/input.hpp"
#include "vts/parameter.hpp"

namespace vts {

struct MouseState {
	int x = 0;
	int y = 0;
};

using ConstParameterView =
    decltype(std::declval<const std::unordered_map<std::string, Parameter>&>()
             | std::views::values);

class ParameterManager {
private:
	MouseState _mouse;

	std::unordered_map<std::string, Parameter> _params;

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);

public:
	ParameterManager() = default;
	Parameter& operator[](const char* name);
	ConstParameterView values() const;
	void add(const ParameterData& data);
	void handleEvent(SDL_UserEvent& event);
};

}  // namespace vts

#endif  // VTS_PARAMETER_MANAGER_HPP_
