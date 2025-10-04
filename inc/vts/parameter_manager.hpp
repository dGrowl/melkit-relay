#ifndef VTS_PARAMETER_MANAGER_HPP_
#define VTS_PARAMETER_MANAGER_HPP_

#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>

#include "impulse/code.hpp"
#include "vts/parameter.hpp"

namespace vts {

using ParameterStore = std::unordered_map<std::string, Parameter>;

using ParameterView =
    decltype(std::declval<ParameterStore&>() | std::views::values);

class ParameterManager {
private:
	Parameter      _sample;
	ParameterStore _parameters;

public:
	ParameterManager();
	ParameterManager(ParameterManager&)            = delete;
	ParameterManager& operator=(ParameterManager&) = delete;

	bool                     isEmpty() const;
	Parameter&               getSample();
	ParameterStore::iterator end();
	ParameterStore::iterator find(const std::string& name);
	ParameterView            values();
	void                     add(const std::string& name);
	void                     clear();
	void                     distributeImpulse(imp::Code code, float value);
};

}  // namespace vts

#endif  // VTS_PARAMETER_MANAGER_HPP_
