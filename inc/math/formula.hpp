#ifndef MATH_FORMULA_HPP_
#define MATH_FORMULA_HPP_

namespace math {

template <typename T>
T remapLinear(const T inValue,
              const T inLower,
              const T inUpper,
              const T outLower,
              const T outUpper) {
	if (inUpper == inLower) {
		return 0;
	}
	return outLower
	       + (inValue - inLower)
	       * (outUpper - outLower)
	       / (inUpper - inLower);
}

}  // namespace math

#endif  // MATH_FORMULA_HPP_
