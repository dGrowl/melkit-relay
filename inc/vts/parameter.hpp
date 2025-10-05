#ifndef VTS_PARAMETER_HPP_
#define VTS_PARAMETER_HPP_

#include <string>
#include <unordered_map>

#include <SDL3/SDL_stdinc.h>

#include "impulse/code.hpp"
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
	explicit Parameter(const std::string& name);

	BlendMode                 getBlendMode() const;
	bool                      hasImpulses() const;
	bool                      isFresh();
	const imp::Receiver&      getReceiver(imp::Code code) const;
	const ImpulseReceiverMap& getReceivers() const;
	const std::string&        getName() const;
	float                     getMax() const;
	float                     getMin() const;
	float                     getNormalized() const;
	float                     getOutput() const;
	ImpulseReceiverMap&       getReceivers();
	void                      addImpulse(imp::Code code, bool isInverted = false);
	void                      clearImpulses();
	void                      handleImpulse(imp::Code code, float value);
	void                      removeImpulse(imp::Code code);
	void                      setBlendMode(BlendMode mode);
	void                      setName(const std::string& name);
	void                      updateBounds();
};

}  // namespace vts

#endif  // VTS_PARAMETER_HPP_
