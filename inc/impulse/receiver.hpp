#ifndef IMPULSE_RECEIVER_HPP_
#define IMPULSE_RECEIVER_HPP_

#include "impulse/code.hpp"

namespace imp {

class Receiver {
private:
	const Code _code;

	bool  _isInverted;
	float _inMax;
	float _inMin;
	float _outMax;
	float _outMin;
	float _value;

public:
	explicit Receiver(const Code code, const bool isInverted = false);

	[[nodiscard]] bool  getIsInverted() const;
	[[nodiscard]] Code  getCode() const;
	[[nodiscard]] float getMax() const;
	[[nodiscard]] float getMin() const;
	[[nodiscard]] float getValue() const;

	bool& isInvertedRef();
	void  update(float value);
};

}  // namespace imp

#endif  // IMPULSE_RECEIVER_HPP_
