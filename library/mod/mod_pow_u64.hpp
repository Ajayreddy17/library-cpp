#ifndef AJAY_MOD_POW_U64
#define AJAY_MOD_POW_U64

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_pow_u64.hpp
// a^n mod 2^64
unsigned long long mod_pow_u64(unsigned long long a, unsigned long long n) {
    unsigned long long x = 1;
    while (n) {
        if (n & 1) x *= a;
        a *= a, n >>= 1;
    }
    return x;
}
} // namespace mitsuha
#endif // AJAY_MOD_POW_U64
