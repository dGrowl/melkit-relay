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

bool Parameter::hasInputs() const {
	return !_inputs.empty();
}

const std::string& Parameter::getName() const {
	return _name;
}

InputMap& Parameter::getInputs() {
	return _inputs;
};

const InputMap& Parameter::getInputs() const {
	return _inputs;
};

float Parameter::getNormalized() const {
	return (_output - min) / (max - min);
}

float Parameter::getOutput() const {
	return _output;
}

void Parameter::addInput(const InputData& data) {
	_inputs.emplace(data.getId(), std::move(data));
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

void Parameter::handleInput(const InputId id, const float value) {
	auto input = _inputs.find(id);
	if (input == _inputs.end()) {
		return;
	}
	auto& data = input->second;
	data.value = std::clamp(value, data.min, data.max);
	float nextOutput = defaultValue;
	for (InputData& data : _inputs | std::views::values) {
		float remappedValue = remap(data.value, data.min, data.max, min, max);
		if (std::abs(nextOutput - defaultValue)
		    < std::abs(remappedValue - defaultValue)) {
			nextOutput = remappedValue;
		}
	}
	_output = nextOutput;
}

void Parameter::removeInput(const InputId id) {
	_inputs.erase(id);
}

void Parameter::setInputs(const std::vector<InputData>& inputs) {
	for (const auto& input : inputs) {
		_inputs.insert_or_assign(input.getId(), input);
	}
}

}  // namespace vts
