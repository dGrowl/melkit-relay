#include "vts/parameter.hpp"

#include <SDL3/SDL_events.h>
#include <algorithm>
#include <libuiohook/uiohook.h>
#include <ranges>
#include <tuple>

#include "mnk/event.hpp"
#include "vts/response.hpp"

namespace vts {

static constexpr const char* DEFAULT_PARAMETER_NAME = "MK_NewParameter";

float Parameter::calcInputSum() {
	float total = 0;
	for (imp::InputData& data : _inputs | std::views::values) {
		total += data.getValue();
	}
	return total;
}

float Parameter::calcMajorInput() {
	float majorValue = 0;
	float majorDelta = 0;
	for (imp::InputData& data : _inputs | std::views::values) {
		const float delta = std::abs(data.getValue() - _defaultValue);
		if ((delta > majorDelta)
		    || (delta == majorDelta && data.getValue() > majorValue)) {
			majorValue = data.getValue();
			majorDelta = delta;
		}
	}
	return majorValue;
}

void Parameter::updateOutput() {
	float newOutput = 0;
	switch (_blendMode) {
		case BlendMode::MAX:
			newOutput = calcMajorInput();
			break;
		case BlendMode::BOUNDED_SUM:
			newOutput = calcInputSum();
			newOutput = std::clamp(newOutput, _min, _max);
			break;
	}
	if (_output == newOutput) {
		return;
	}
	_output = newOutput;
	_fresh  = true;
}

Parameter::Parameter() :
    Parameter(DEFAULT_PARAMETER_NAME) {}

Parameter::Parameter(const std::string& name) :
    _blendMode(BlendMode::MAX),
    _fresh(false),
    _defaultValue(0.0f),
    _max(1.0f),
    _min(0.0f),
    _output(0.0f),
    _inputs(),
    _name(name) {}

BlendMode Parameter::getBlendMode() const {
	return _blendMode;
}

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

const imp::InputData& Parameter::getInput(const imp::InputId id) const {
	return _inputs.at(id);
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

float Parameter::getMax() const {
	return _max;
}

float Parameter::getMin() const {
	return _min;
}

float Parameter::getNormalized() const {
	return (_output - _min) / (_max - _min);
}

float Parameter::getOutput() const {
	return _output;
}

void Parameter::addInput(const imp::InputId id, const bool isInverted) {
	_inputs.emplace(std::piecewise_construct,
	                std::forward_as_tuple(id),
	                std::forward_as_tuple(id, isInverted));
	updateBounds();
}

void Parameter::clearInputs() {
	_inputs.clear();
	updateBounds();
}

void Parameter::handleInput(const imp::InputId id, const float value) {
	auto input = _inputs.find(id);
	if (input == _inputs.end()) {
		return;
	}
	input->second.update(value);
	updateOutput();
}

void Parameter::removeInput(const imp::InputId id) {
	_inputs.erase(id);
	updateBounds();
}

void Parameter::setBlendMode(const BlendMode mode) {
	_blendMode = mode;
	updateBounds();
}

void Parameter::setName(const std::string& name) {
	_name = name;
}

void Parameter::updateBounds() {
	if (_inputs.empty()) {
		_max = 1.0f;
		_min = 0.0f;
		return;
	}
	_max = _defaultValue;
	_min = _defaultValue;
	for (const auto& input : _inputs | std::views::values) {
		_max = std::max(_max, input.getMax());
		_min = std::min(_min, input.getMin());
	}
}

}  // namespace vts
