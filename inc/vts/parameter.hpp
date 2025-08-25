#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>

#include "vts/input.hpp"

namespace vts {

struct ParameterData {
	std::string name;
	float defaultValue;
	float max;
	float min;
};

class Parameter {
private:
	std::string _name;
	std::unordered_map<Uint32, float> _inputs;

	float _defaultValue;
	float _max;
	float _min;
	float _output;

public:
	Parameter();
	Parameter(const ParameterData& data);
	const std::string& getName() const;
	float getOutput() const;
	void handleInput(const Uint32 id, const float value);
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
