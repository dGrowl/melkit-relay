#ifndef MATH_GEOMETRY_HPP_
#define MATH_GEOMETRY_HPP_

namespace math {

template <typename T>
struct Vector2 {
	T x = 0;
	T y = 0;
};

template <typename T>
struct Rectangle {
	T top    = 0;
	T left   = 0;
	T bottom = 0;
	T right  = 0;
};

}  // namespace math

#endif  // MATH_GEOMETRY_HPP_
