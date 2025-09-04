#include "vts/input.hpp"

namespace vts {

InputData::InputData(const InputId id) :
    _id(id),
    min(0.0f),
    max(1.0f),
    value(0.0f) {
	const InputId event = id & 0xFFFF;
	if (event == MOUSE_MOVE_REL) {
		min = -64.0f;
		max = 64.0f;
	}
};

}  // namespace vts
