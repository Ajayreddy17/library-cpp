#ifndef AJAY_BIT_UTILS
#define AJAY_BIT_UTILS

namespace mitsuha {
    template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr>
    __attribute__((target("popcnt"))) constexpr int popcount(const T x) { return _mm_popcnt_u32(x); }
    template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
    __attribute__((target("popcnt"))) constexpr int popcount(const T x) { return _mm_popcnt_u64(x); }
    template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr>
    constexpr int msb(const T x) { return (x == 0 ? -1 : bit_num<T> - 1 - __builtin_clz(x)); }
    template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
    constexpr int msb(const T x) { return (x == 0 ? -1 : bit_num<T> - 1 - __builtin_clzll(x)); }
    template <typename T, std::enable_if_t<std::negation_v<is_nbit<T, 64>>, std::nullptr_t> = nullptr> 
    constexpr int lsb(const T x) { return (x == 0 ? -1 : __builtin_ctz(x)); }
    template <typename T, std::enable_if_t<is_nbit_v<T, 64>, std::nullptr_t> = nullptr>
    constexpr int lsb(const T x) { return (x == 0 ? -1 : __builtin_ctzll(x)); }

    template <typename T> constexpr int kbit(const T x, const unsigned int k) { return (x >> k) & 1; }
} // namespace mitsuha
#endif // AJAY_BIT_UTILS