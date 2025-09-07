#include <algorithm>

#include "vts/input.hpp"

namespace vts {

InputData::InputData(const InputId id) :
    _id(id),
    _inMax(1.0f),
    _inMin(0.0f),
    _outMax(1.0f),
    _outMin(0.0f),
    _value(0.0f),
    isInverted(false) {
	const InputId event = id & 0xFFFF;
	if (event == MOUSE_MOVE_REL) {
		_inMin  = -64.0f;
		_inMax  = 64.0f;
		_outMin = -1.0f;
	}
};

float InputData::getMax() const {
	return isInverted ? -_outMin : _outMax;
}

float InputData::getMin() const {
	return isInverted ? -_outMax : _outMin;
}

float InputData::getValue() const {
	return isInverted ? _value * -1.0f : _value;
}

InputId InputData::getId() const {
	return _id;
}

float remap(float inValue,
            float inLower,
            float inUpper,
            float outLower,
            float outUpper) {
	return outLower
	       + (inValue - inLower)
	       * (outUpper - outLower)
	       / (inUpper - inLower);
}

void InputData::update(float inValue) {
	inValue = std::clamp(inValue, _inMin, _inMax);
	inValue = remap(inValue, _inMin, _inMax, _outMin, _outMax);
	_value  = inValue;
}

}  // namespace vts
