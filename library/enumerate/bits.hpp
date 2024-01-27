#ifndef AJAY_ENUMERATE_BITS
#define AJAY_ENUMERATE_BITS

namespace mitsuha{
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
    int p = (b[L] ? L : b._Find_next(L));
    while (p < R) {
        f(p);
        p = b._Find_next(p);
    }
}
} // namespace mitsuha
#endif // AJAY_ENUMERATE_BITS