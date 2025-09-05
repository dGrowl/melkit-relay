#include <algorithm>

#include "vts/input.hpp"

namespace vts {

InputData::InputData(const InputId id) :
    _id(id),
    _inMin(0.0f),
    _inMax(1.0f),
    outMin(0.0f),
    outMax(1.0f),
    value(0.0f) {
	const InputId event = id & 0xFFFF;
	if (event == MOUSE_MOVE_REL) {
		_inMin = -64.0f;
		_inMax = 64.0f;
		outMin = -1.0f;
	}
};

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
	inValue = remap(inValue, _inMin, _inMax, outMin, outMax);
	value = inValue;
}

}  // namespace vts
