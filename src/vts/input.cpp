#include <algorithm>
#include <limits>

#include "math/formula.hpp"
#include "vts/input.hpp"

namespace vts {

InputData::InputData(const InputId id, const bool isInverted) :
    _id(id),
    _isInverted(isInverted),
    _inMax(1.0f),
    _inMin(0.0f),
    _outMax(1.0f),
    _outMin(0.0f),
    _value(0.0f) {
	const InputId event = id & 0xFFFF;
	switch (event) {
		case MOUSE_MOVE_REL:
			_inMin  = -64.0f;
			_inMax  = 64.0f;
			_outMin = -1.0f;
			break;
		case GAMEPAD_STICK_RIGHT:
		case GAMEPAD_STICK_LEFT:
			_inMin  = std::numeric_limits<Sint16>::min();
			_inMax  = std::numeric_limits<Sint16>::max();
			_outMin = -1.0f;
			break;
		case GAMEPAD_TRIGGER:
			_inMax = std::numeric_limits<Sint16>::max();
			break;
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
	inValue = std::clamp(inValue, _inMin, _inMax);
	inValue = math::remapLinear(inValue, _inMin, _inMax, _outMin, _outMax);
	_value  = inValue;
}

}  // namespace vts
