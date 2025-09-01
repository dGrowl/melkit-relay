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

using InputMap = std::unordered_map<InputId, InputData>;

class Parameter {
private:
	std::string _name;
	InputMap _inputs;
	float _output;

public:
	float defaultValue;
	float max;
	float min;

	Parameter();
	Parameter(const ParameterData& data);
	bool hasInputs() const;
	const std::string& getName() const;
	float getNormalized() const;
	float getOutput() const;
	InputMap& getInputs();
	void addInput(const InputData& data);
	void handleInput(const InputId id, const float value);
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
