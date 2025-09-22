#ifndef VTS_PARAMETER_MANAGER_HPP_
#define VTS_PARAMETER_MANAGER_HPP_

#include <ranges>
#include <string>
#include <unordered_map>

#include <SDL3/SDL_events.h>

#include "math/geometry.hpp"
#include "vts/input.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace vts {

struct MouseState {
	int   x  = 0;
	int   y  = 0;
	float dx = 0;
	float dy = 0;
};

using ParameterStore = std::unordered_map<std::string, Parameter>;

using ParameterView =
    decltype(std::declval<ParameterStore&>() | std::views::values);

class ParameterManager {
private:
	float                _mouseCoefficient;
	int                  _mouseSensitivity;
	math::Rectangle<int> _mouseBounds;
	MouseState           _mouseState;
	Uint64               _lastUpdateTimeMs;

	Parameter      _sample;
	ParameterStore _parameters;

	void handleGamepadAxisMotion(SDL_GamepadAxisEvent& event);
	void handleGamepadButton(SDL_GamepadButtonEvent& event, const bool isPressed);

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);
	void updateMouseMotionCoefficient();

public:
	ParameterManager();
	ParameterManager(ParameterManager&)            = delete;
	ParameterManager& operator=(ParameterManager&) = delete;

	bool                        isEmpty() const;
	const math::Rectangle<int>& getMouseBounds() const;
	const MouseState&           getMouseState() const;
	int                         getMouseSensitivity() const;
	Parameter&                  getSample();
	ParameterStore::iterator    end();
	ParameterStore::iterator    find(const std::string& name);
	ParameterView               values();
	void                        add(const std::string& name);
	void                        clear();
	void                        handleGamepadEvent(SDL_Event&           event,
	                                               const SDL_JoystickID activeGamepadId);
	void                        handleEvent(SDL_UserEvent& event);
	void                        setMouseBounds(const math::Rectangle<int>& bounds);
	void                        setMouseSensitivity(const int sensitivity);
	void                        update();
};

}  // namespace vts

#endif  // VTS_PARAMETER_MANAGER_HPP_
