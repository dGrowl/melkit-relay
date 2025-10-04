#include "vts/parameter_manager.hpp"

#include <ranges>
#include <string>

#include "impulse/code.hpp"

namespace vts {

ParameterManager::ParameterManager() :
    _sample(),
    _parameters() {}

bool ParameterManager::isEmpty() const {
	return _parameters.empty();
}

ParameterStore::iterator ParameterManager::end() {
	return _parameters.end();
}

ParameterStore::iterator ParameterManager::find(const std::string& name) {
	return _parameters.find(name);
}

Parameter& ParameterManager::getSample() {
	return _sample;
}

auto ParameterManager::values() -> ParameterView {
	return _parameters | std::views::values;
}

void ParameterManager::add(const std::string& name) {
	_parameters.emplace(name, name);
}

void ParameterManager::clear() {
	_parameters.clear();
}

void ParameterManager::distributeImpulse(imp::Code code, float value) {
	for (auto& parameter : values()) {
		parameter.handleImpulse(code, value);
	}
	_sample.handleImpulse(code, value);
}

}  // namespace vts
