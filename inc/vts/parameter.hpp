#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "vts/input.hpp"

namespace vts {

struct ParameterData {
	std::string name;
	float defaultValue = 0.0f;
	float max = 1.0f;
	float min = 0.0f;
};

using InputMap = std::unordered_map<InputId, InputData>;

class Parameter {
private:
	std::string _name;
	InputMap _inputs;
	float _output;
	bool _fresh;

public:
	float defaultValue;
	float max;
	float min;

	Parameter();
	Parameter(const ParameterData& data);

	bool hasInputs() const;
	bool isFresh();
	const InputMap& getInputs() const;
	const std::string& getName() const;
	float getNormalized() const;
	float getOutput() const;
	InputMap& getInputs();
	void addInput(const InputData& data);
	void handleInput(const InputId id, const float value);
	void removeInput(const InputId id);
	void setInputs(const std::vector<InputData>& inputs);
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
