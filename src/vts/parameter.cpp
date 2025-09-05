#include <algorithm>
#include <ranges>

#include <SDL3/SDL_events.h>
#include <libuiohook/uiohook.h>

#include "mnk/event.hpp"
#include "vts/parameter.hpp"
#include "vts/response.hpp"

namespace vts {

static constexpr const char* DEFAULT_PARAMETER_NAME = "MK_DEFAULT_NAME";

Parameter::Parameter() :
    _name(DEFAULT_PARAMETER_NAME),
    _inputs(),
    _output(0.0f),
    _fresh(false),
    defaultValue(0.0f),
    max(1.0f),
    min(0.0f) {}

Parameter::Parameter(const ParameterData& data) :
    _name(data.name),
    _inputs(),
    _output(0.0f),
    _fresh(false),
    defaultValue(data.defaultValue),
    max(data.max),
    min(data.min) {}

bool Parameter::hasInputs() const {
	return !_inputs.empty();
}

bool Parameter::isFresh() {
	if (_fresh) {
		_fresh = false;
		return true;
	}
	return false;
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

void Parameter::handleInput(const InputId id, const float value) {
	auto input = _inputs.find(id);
	if (input == _inputs.end()) {
		return;
	}
	input->second.update(value);
	float newOutput = defaultValue;
	for (InputData& data : _inputs | std::views::values) {
		if (std::abs(newOutput - defaultValue)
		    < std::abs(data.value - defaultValue)) {
			newOutput = data.value;
		}
	}
	if (_output == newOutput) {
		return;
	}
	_output = newOutput;
	_fresh = true;
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
