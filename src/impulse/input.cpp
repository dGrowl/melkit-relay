#include "impulse/input.hpp"

#include <algorithm>
#include <limits>

#include "math/formula.hpp"

namespace imp {

InputData::InputData(const InputId id, const bool isInverted) :
    _id(id),
    _isInverted(isInverted),
    _inMax(1.0f),
    _inMin(0.0f),
    _outMax(1.0f),
    _outMin(0.0f),
    _value(0.0f) {
	const EventTag event = id & 0xFFFF;
	if ((event == InputEvent::MOUSE_MOVE_REL)
	    || (event == InputEvent::GAMEPAD_STICK_RIGHT)
	    || (event == InputEvent::GAMEPAD_STICK_LEFT)) {
		_outMin = -1.0f;
	}
};

bool InputData::getIsInverted() const {
	return _isInverted;
}

bool& InputData::isInvertedRef() {
	return _isInverted;
}

float InputData::getMax() const {
	return _isInverted ? -_outMin : _outMax;
}

float InputData::getMin() const {
	return _isInverted ? -_outMax : _outMin;
}

float InputData::getValue() const {
	return _isInverted ? _value * -1.0f : _value;
}

InputId InputData::getId() const {
	return _id;
}

void InputData::update(float inValue) {
	_value = inValue;
}

}  // namespace imp
