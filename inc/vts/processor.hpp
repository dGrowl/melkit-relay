#ifndef VTS_PROCESSOR_HPP_
#define VTS_PROCESSOR_HPP_

#include <ranges>
#include <vector>

#include <SDL3/SDL_events.h>

#include "math/geometry.hpp"
#include "vts/input.hpp"
#include "vts/parameter_manager.hpp"
#include "ws/controller.hpp"

namespace vts {

using ImpulseQueue = std::vector<std::pair<vts::InputId, float>>;

struct MouseState {
	int   x         = 0;
	int   y         = 0;
	float dx        = 0.0f;
	float dy        = 0.0f;
	float wheelUp   = 0.0f;
	float wheelDown = 0.0f;
};

class Processor {
private:
	float                _mouseCoefficient;
	int                  _mouseSensitivity;
	math::Rectangle<int> _mouseBounds;
	MouseState           _mouseState;
	Uint64               _lastUpdateTimeMs;

	ImpulseQueue _queue;

	void handleGamepadAxisMotion(SDL_GamepadAxisEvent& event);
	void handleGamepadButton(SDL_GamepadButtonEvent& event, const bool isPressed);

	void handleKeyDown(SDL_UserEvent& event);
	void handleKeyUp(SDL_UserEvent& event);
	void handleMouseButton(SDL_UserEvent& event, bool isClicked);
	void handleMouseMove(SDL_UserEvent& event);
	void handleMouseWheel(SDL_UserEvent& event);

	void updateMouseMovement(Uint64 dtMs);
	void updateMouseWheel(Uint64 dtMs);

public:
	Processor();
	Processor(Processor&)            = delete;
	Processor& operator=(Processor&) = delete;

	const ImpulseQueue&         impulses() const;
	const math::Rectangle<int>& getMouseBounds() const;
	const MouseState&           getMouseState() const;
	int                         getMouseSensitivity() const;
	void                        clear();
	void                        handleGamepadEvent(SDL_Event&           event,
	                                               const SDL_JoystickID activeGamepadId);
	void                        handleEvent(SDL_UserEvent& event);
	void                        setMouseBounds(const math::Rectangle<int>& bounds);
	void                        setMouseSensitivity(const int sensitivity);
	void                        update();
};

}  // namespace vts

#endif  // VTS_PROCESSOR_HPP_
