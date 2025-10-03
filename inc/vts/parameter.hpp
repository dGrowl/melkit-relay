#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>
#include <vector>

#include "impulse/receiver.hpp"

namespace vts {

enum class BlendMode : Uint8 {
	MAX,
	BOUNDED_SUM,
};

using ImpulseReceiverMap = std::unordered_map<imp::Code, imp::Receiver>;

class Parameter {
private:
	BlendMode          _blendMode;
	bool               _fresh;
	float              _defaultValue;
	float              _max;
	float              _min;
	float              _output;
	ImpulseReceiverMap _impulseReceivers;
	std::string        _name;

	float calcImpulseSum();
	float calcMajorImpulse();
	void  updateOutput();

public:
	Parameter();
	Parameter(const std::string& name);

	BlendMode                 getBlendMode() const;
	bool                      hasImpulses() const;
	bool                      isFresh();
	const imp::Receiver&      getReceiver(const imp::Code code) const;
	const ImpulseReceiverMap& getReceivers() const;
	const std::string&        getName() const;
	float                     getMax() const;
	float                     getMin() const;
	float                     getNormalized() const;
	float                     getOutput() const;
	ImpulseReceiverMap&       getReceivers();
	void addImpulse(const imp::Code code, const bool isInverted = false);
	void clearImpulses();
	void handleImpulse(const imp::Code code, const float value);
	void removeImpulse(const imp::Code code);
	void setBlendMode(const BlendMode mode);
	void setName(const std::string& name);
	void updateBounds();
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
