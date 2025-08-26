#include <SDL3/SDL_events.h>
#include <libuiohook/uiohook.h>

#include "mnk/event.hpp"
#include "vts/response.hpp"

namespace vts {

static constexpr const char* DEFAULT_PARAMETER_NAME = "MK_DEFAULT_NAME";

// ParameterData::ParameterData(const char* cName):
// 	name(cName) {}

Parameter::Parameter() :
    _name(DEFAULT_PARAMETER_NAME),
    _inputs(),
    defaultValue(0.0f),
    max(1.0f),
    min(0.0f),
    output(0.0f) {}

Parameter::Parameter(const ParameterData& data) :
    _name(data.name),
    _inputs(),
    defaultValue(data.defaultValue),
    max(data.max),
    min(data.min),
    output(0.0f) {}

const std::string& Parameter::getName() const {
	return _name;
}

float Parameter::getOutput() const {
	return output;
}

void Parameter::handleInput(const Uint32 id, const float value) {
	auto input = _inputs.find(id);
	if (input == _inputs.end()) {
		return;
	}
	input->second = value;
}

}  // namespace vts
