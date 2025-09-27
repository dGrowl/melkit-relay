#include "vts/parameter_manager.hpp"

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

void ParameterManager::distributeImpulse(InputId id, float value) {
	for (auto& parameter : values()) {
		parameter.handleInput(id, value);
	}
	_sample.handleInput(id, value);
}

}  // namespace vts
