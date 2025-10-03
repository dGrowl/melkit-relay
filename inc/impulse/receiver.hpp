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
	Receiver(const Code code, const bool isInverted = false);

	bool  getIsInverted() const;
	bool& isInvertedRef();
	float getMax() const;
	float getMin() const;
	float getValue() const;
	Code  getCode() const;
	void  update(float inValue);
};

}  // namespace imp

#endif  // IMPULSE_RECEIVER_HPP_
