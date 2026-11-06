#ifndef AJAY_ENUMERATE_BITS
#define AJAY_ENUMERATE_BITS

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/enumerate/bits.hpp
template <typename F>
void enumerate_bits_32(unsigned int s, F f) {
    while (s) {
        int i = __builtin_ctz(s);
        f(i);
        s ^= 1 << i;
    }
}

template <typename F>
void enumerate_bits_64(unsigned long long s, F f) {
    while (s) {
        int i = __builtin_ctzll(s);
        f(i);
        s ^= 1ULL << i;
    }
}

template <typename BS, typename F>
void enumerate_bits_bitset(BS& b, int L, int R, F f) {
    if (L >= (int)b.size()) return;
    int p = (b[L] ? L : b._Find_next(L));
    while (p < R) {
        f(p);
        p = b._Find_next(p);
    }
}

// f(i) for each set bit i of s, ascending
template <typename UINT, typename F>
inline void enumerate_all_bit(UINT s, F&& f) {
    static_assert(is_unsigned<UINT>::value);
    while (s) {
        f(__builtin_ctzll(s));
        s &= s - 1;
    }
}

// f(t) for each nonempty submask t of s, descending; then f(0) if inc_empty
template <typename UINT, bool inc_empty, typename F>
inline void enumerate_all_subset(UINT s, F&& f) {
    static_assert(is_unsigned<UINT>::value);
    for (UINT t = s; t; t = (t - 1) & s) f(t);
    if constexpr (inc_empty) f(0);
}
} // namespace mitsuha
#endif // AJAY_ENUMERATE_BITS