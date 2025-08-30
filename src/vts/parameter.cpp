#include <algorithm>
#include <ranges>

#include <SDL3/SDL_events.h>
#include <libuiohook/uiohook.h>

#include "mnk/event.hpp"
#include "vts/response.hpp"

namespace vts {

static constexpr const char* DEFAULT_PARAMETER_NAME = "MK_DEFAULT_NAME";

Parameter::Parameter() :
    _name(DEFAULT_PARAMETER_NAME),
    _inputs(),
    _output(0.0f),
    defaultValue(0.0f),
    max(1.0f),
    min(0.0f) {}

Parameter::Parameter(const ParameterData& data) :
    _name(data.name),
    _inputs(),
    _output(0.0f),
    defaultValue(data.defaultValue),
    max(data.max),
    min(data.min) {}

const std::string& Parameter::getName() const {
	return _name;
}

InputMap& Parameter::getInputs() {
	return _inputs;
};

float Parameter::getNormalized() const {
	return (_output - min) / (max - min);
}

float Parameter::getOutput() const {
	return _output;
}

void Parameter::addInput(const InputId id) {
	_inputs.emplace(id, 0.0f);
}

void Parameter::handleInput(const Uint32 id, const float value) {
	auto input = _inputs.find(id);
	if (input == _inputs.end()) {
		return;
	}
	input->second = value;
	_output = std::ranges::max(_inputs | std::views::values);
}

}  // namespace vts
