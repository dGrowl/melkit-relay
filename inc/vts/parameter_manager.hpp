#ifndef VTS_PARAMETER_MANAGER_HPP_
#define VTS_PARAMETER_MANAGER_HPP_

#include <ranges>
#include <string>
#include <unordered_map>

#include <SDL3/SDL_events.h>

#include "vts/input.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace vts {

struct MouseState {
	int x = 0;
	int y = 0;
	float dx = 0;
	float dy = 0;
};

using ParameterView =
    decltype(std::declval<std::unordered_map<std::string, Parameter>&>()
             | std::views::values);

class ParameterManager {
private:
	ws::IController& _wsController;

	MouseState _mouse;
	Parameter _sample;
	std::unordered_map<std::string, Parameter> _params;

	Uint64 _lastUpdateTimeMs;
	Uint64 _nextUpdateTimeMs;

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);

public:
	ParameterManager(ws::IController& wsController);
	Parameter& operator[](const char* name);

	std::unordered_map<std::string, Parameter>::iterator end();
	std::unordered_map<std::string, Parameter>::iterator find(
	    const std::string& name);
	Parameter& getSample();
	ParameterView values();
	void add(const ParameterData& data);
	void clear();
	void handleEvent(SDL_UserEvent& event);
	void update();
};

}  // namespace vts

#endif  // VTS_PARAMETER_MANAGER_HPP_
