#include "vts/parameter.hpp"

#include <algorithm>
#include <cmath>
#include <ranges>
#include <tuple>
#include <utility>

#include "impulse/code.hpp"
#include "impulse/receiver.hpp"

namespace vts {

static constexpr const char* DEFAULT_PARAMETER_NAME = "MK_NewParameter";

float Parameter::calcImpulseSum() {
	float total = 0;
	for (const auto& receiver : _impulseReceivers | std::views::values) {
		total += receiver.getValue();
	}
	return total;
}

float Parameter::calcMajorImpulse() {
	float majorValue = 0;
	float majorDelta = 0;
	for (const auto& receiver : _impulseReceivers | std::views::values) {
		const float delta = std::abs(receiver.getValue() - _defaultValue);
		if ((delta > majorDelta)
		    || (delta == majorDelta && receiver.getValue() > majorValue)) {
			majorValue = receiver.getValue();
			majorDelta = delta;
		}
	}
	return majorValue;
}

void Parameter::updateOutput() {
	float newOutput = 0;
	switch (_blendMode) {
		case BlendMode::MAX:
			newOutput = calcMajorImpulse();
			break;
		case BlendMode::BOUNDED_SUM:
			newOutput = calcImpulseSum();
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
    _defaultValue(0.0F),
    _max(1.0F),
    _min(0.0F),
    _output(0.0F),
    _impulseReceivers(),
    _name(name) {}

BlendMode Parameter::getBlendMode() const {
	return _blendMode;
}

bool Parameter::hasImpulses() const {
	return !_impulseReceivers.empty();
}

bool Parameter::isFresh() {
	if (_fresh) {
		_fresh = false;
		return true;
	}
	return false;
}

const imp::Receiver& Parameter::getReceiver(const imp::Code code) const {
	return _impulseReceivers.at(code);
}

const std::string& Parameter::getName() const {
	return _name;
}

ImpulseReceiverMap& Parameter::getReceivers() {
	return _impulseReceivers;
};

const ImpulseReceiverMap& Parameter::getReceivers() const {
	return _impulseReceivers;
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

void Parameter::addImpulse(const imp::Code code, const bool isInverted) {
	_impulseReceivers.emplace(std::piecewise_construct,
	                          std::forward_as_tuple(code),
	                          std::forward_as_tuple(code, isInverted));
	updateBounds();
}

void Parameter::clearImpulses() {
	_impulseReceivers.clear();
	updateBounds();
}

void Parameter::handleImpulse(const imp::Code code, const float value) {
	auto receiver = _impulseReceivers.find(code);
	if (receiver == _impulseReceivers.end()) {
		return;
	}
	receiver->second.update(value);
	updateOutput();
}

void Parameter::removeImpulse(const imp::Code code) {
	_impulseReceivers.erase(code);
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
	if (_impulseReceivers.empty()) {
		_max = 1.0F;
		_min = 0.0F;
		return;
	}
	_max = _defaultValue;
	_min = _defaultValue;
	for (const auto& receiver : _impulseReceivers | std::views::values) {
		_max = std::max(_max, receiver.getMax());
		_min = std::min(_min, receiver.getMin());
	}
}

}  // namespace vts
