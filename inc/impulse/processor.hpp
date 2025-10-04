#ifndef IMPULSE_PROCESSOR_HPP_
#define IMPULSE_PROCESSOR_HPP_

#include <utility>
#include <vector>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_stdinc.h>

#include "impulse/code.hpp"
#include "math/geometry.hpp"

namespace imp {

using Impulse      = std::pair<imp::Code, float>;
using ImpulseQueue = std::vector<Impulse>;

struct MouseState {
	int   x         = 0;
	int   y         = 0;
	float dx        = 0.0F;
	float dy        = 0.0F;
	float wheelUp   = 0.0F;
	float wheelDown = 0.0F;
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

}  // namespace imp

#endif  // IMPULSE_PROCESSOR_HPP_
