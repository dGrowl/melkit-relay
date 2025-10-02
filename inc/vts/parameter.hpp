#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "impulse/input.hpp"

namespace vts {

enum class BlendMode : Uint8 {
	MAX,
	BOUNDED_SUM,
};

using InputMap = std::unordered_map<imp::InputId, imp::InputData>;

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
	Parameter(const std::string& name);

	BlendMode             getBlendMode() const;
	bool                  hasInputs() const;
	bool                  isFresh();
	const imp::InputData& getInput(const imp::InputId id) const;
	const InputMap&       getInputs() const;
	const std::string&    getName() const;
	float                 getMax() const;
	float                 getMin() const;
	float                 getNormalized() const;
	float                 getOutput() const;
	InputMap&             getInputs();
	void addInput(const imp::InputId id, const bool isInverted = false);
	void clearInputs();
	void handleInput(const imp::InputId id, const float value);
	void removeInput(const imp::InputId id);
	void setBlendMode(const BlendMode mode);
	void setName(const std::string& name);
	void updateBounds();
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
