#include "impulse/receiver.hpp"

#include "impulse/code.hpp"

namespace imp {

Receiver::Receiver(const Code code, const bool isInverted) :
    _code(code),
    _isInverted(isInverted),
    _inMax(1.0f),
    _inMin(0.0f),
    _outMax(1.0f),
    _outMin(0.0f),
    _value(0.0f) {
	const EventTag::T event = code & 0xFFFF;
	if ((event == EventTag::MOUSE_MOVE_REL)
	    || (event == EventTag::GAMEPAD_STICK_RIGHT)
	    || (event == EventTag::GAMEPAD_STICK_LEFT)) {
		_outMin = -1.0f;
	}
};

bool Receiver::getIsInverted() const {
	return _isInverted;
}

bool& Receiver::isInvertedRef() {
	return _isInverted;
}

float Receiver::getMax() const {
	return _isInverted ? -_outMin : _outMax;
}

float Receiver::getMin() const {
	return _isInverted ? -_outMax : _outMin;
}

float Receiver::getValue() const {
	return _isInverted ? _value * -1.0f : _value;
}

Code Receiver::getCode() const {
	return _code;
}

void Receiver::update(float value) {
	_value = value;
}

}  // namespace imp
