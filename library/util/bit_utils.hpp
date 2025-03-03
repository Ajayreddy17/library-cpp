#ifndef AJAY_BIT_UTILS
#define AJAY_BIT_UTILS

#include "library/type_traits/constraints.hpp"

#ifdef LOCAL
constexpr uint32_t _mm_popcnt_u32(uint32_t x) { return __builtin_popcount(x); }
constexpr uint64_t _mm_popcnt_u64(uint64_t x) { return __builtin_popcountll(x); }

constexpr uint32_t _pdep_u32(uint32_t val, uint32_t mask) {
    uint32_t res = 0;
    for (uint32_t bb = 1; mask; bb <<= 1) {
        if (val & bb) res |= mask & -mask;
        mask &= mask - 1;
    }
    return res;
}

constexpr uint64_t _pdep_u64(uint64_t val, uint64_t mask) {
    uint64_t res = 0;
    for (uint64_t bb = 1; mask; bb <<= 1) {
        if (val & bb) res |= mask & -mask;
        mask &= mask - 1;
    }
    return res;
}

constexpr uint32_t _blsi_u32(uint32_t x) { return x & -x; }
constexpr uint64_t _blsi_u64(uint64_t x) { return x & -x; }
constexpr uint32_t _tzcnt_u32(uint32_t x) { return __builtin_ctz(x); }
constexpr uint64_t _tzcnt_u64(uint64_t x) { return __builtin_ctzll(x); }
constexpr uint32_t __blsr_u32(uint32_t x) { return x & (x - 1); }
constexpr uint64_t __blsr_u64(uint64_t x) { return x & (x - 1); }
#else
#ifdef _MSC_VER
#include <intrin.h>
#else
#include <x86intrin.h>
#endif
#endif

namespace mitsuha {
template <typename T, typename = std::nullptr_t> struct bitnum { static constexpr int value = 0; };
template <typename T> struct bitnum<T, constraints_t<std::is_integral<T>>> { static constexpr int value = std::numeric_limits<std::make_unsigned_t<T>>::digits; };
template <typename T> static constexpr int bitnum_v = bitnum<T>::value;
template <typename T, size_t n> struct is_nbit { static constexpr bool value = bitnum_v<T> == n; };
template <typename T, size_t n> static constexpr bool is_nbit_v = is_nbit<T, n>::value;

#ifdef LOCAL
template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr>
constexpr int popcount(const T x) { return __builtin_popcount(x); }
template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
constexpr int popcount(const T x) { return __builtin_popcountll(x); }
#else
template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr>
__attribute__((target("popcnt"))) constexpr int popcount(const T x) { return _mm_popcnt_u32(x); }
template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
__attribute__((target("popcnt"))) constexpr int popcount(const T x) { return _mm_popcnt_u64(x); }
#endif
template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr>
constexpr int msb(const T x) { return (x == 0 ? -1 : bitnum_v<T> - 1 - __builtin_clz(x)); }
template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
constexpr int msb(const T x) { return (x == 0 ? -1 : bitnum_v<T> - 1 - __builtin_clzll(x)); }
template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr> 
constexpr int lsb(const T x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
constexpr int lsb(const T x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }
} // namespace mitsuha
#endif // AJAY_BIT_UTILS