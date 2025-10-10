#ifndef CORE_UTILITY_HPP_
#define CORE_UTILITY_HPP_

#include <bit>

namespace core {

template <typename T>
constexpr inline void* unsignedToPointer(const T x) {
	return reinterpret_cast<void*>(static_cast<uintptr_t>(x));
}

template <typename T>
constexpr inline T pointerToUnsigned(const void* p) {
	return static_cast<T>(reinterpret_cast<uintptr_t>(p));
}

template <typename T>
constexpr inline void* signedToPointer(const T x) {
	return reinterpret_cast<void*>(static_cast<intptr_t>(x));
}

template <typename T>
constexpr inline T pointerToSigned(const void* p) {
	return static_cast<T>(reinterpret_cast<intptr_t>(p));
}

constexpr inline void* floatToPointer(float f) {
	uintptr_t bits = std::bit_cast<uint32_t>(f);
	return reinterpret_cast<void*>(static_cast<uintptr_t>(bits));
}

constexpr inline float pointerToFloat(const void* p) {
	uintptr_t bits = reinterpret_cast<uintptr_t>(p);
	return std::bit_cast<float>(static_cast<uint32_t>(bits));
}

}  // namespace core

#endif  // CORE_UTILITY_HPP_
