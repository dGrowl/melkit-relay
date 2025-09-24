#ifndef MATH_FORMULA_HPP_
#define MATH_FORMULA_HPP_

namespace math {

template <typename T>
constexpr T remapLinear(const T inValue,
                        const T inLower,
                        const T inUpper,
                        const T outLower,
                        const T outUpper) {
	if (inUpper == inLower) {
		return T{0};
	}
	return outLower
	       + (inValue - inLower)
	       * (outUpper - outLower)
	       / (inUpper - inLower);
}

template <typename T>
constexpr T remapLinearDeadzone(const T inValue,
                                const T inLower,
                                const T inUpper,
                                const T outLower,
                                const T outUpper,
                                const T deadzone) {
	if (inValue < -deadzone) {
		return remapLinear(inValue, inLower, -deadzone, outLower, T{0});
	}
	if (inValue > deadzone) {
		return remapLinear(inValue, deadzone, inUpper, T{0}, outUpper);
	}
	return T{0};
}

template <typename T>
constexpr T sign(const T x) {
	if (x > T{0})
		return T{1};
	if (x < T{0})
		return T{-1};
	return T{0};
}

}  // namespace math

#endif  // MATH_FORMULA_HPP_
