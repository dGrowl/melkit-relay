#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "vts/input.hpp"

namespace vts {

struct ParameterData {
	std::string name;
	float       defaultValue = 0.0f;
	float       max          = 1.0f;
	float       min          = 0.0f;
};

enum class BlendMode : Uint8 {
	MAX,
	BOUNDED_SUM,
};

using InputMap = std::unordered_map<InputId, InputData>;

class Parameter {
private:
	BlendMode   _blendMode;
	bool        _fresh;
	float       _defaultValue;
	float       _max;
	float       _min;
	float       _output;
	InputMap    _inputs;
	std::string _name;

	float calcInputSum();
	float calcMajorInput();
	void  updateOutput();

public:
	Parameter();
	Parameter(const ParameterData& data);

	BlendMode          getBlendMode() const;
	bool               hasInputs() const;
	bool               isFresh();
	const InputMap&    getInputs() const;
	const std::string& getName() const;
	float              getMax() const;
	float              getMin() const;
	float              getNormalized() const;
	float              getOutput() const;
	InputMap&          getInputs();
	void               addInput(const InputData& data);
	void               handleInput(const InputId id, const float value);
	void               removeInput(const InputId id);
	void               setBlendMode(const BlendMode mode);
	void               setInputs(const std::vector<InputData>& inputs);
	void               updateBounds();
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
