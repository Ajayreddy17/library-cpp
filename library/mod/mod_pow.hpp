#ifndef AJAY_MOD_POW
#define AJAY_MOD_POW

#include "library/mod/montgomery_modint.hpp"
#include "library/mod/barrett.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_pow.hpp
unsigned int mod_pow(int a, long long n, int mod) {
    assert(n >= 0);
    if (mod == 1) return 0;
    a = ((a %= mod) < 0 ? a + mod : a);
    if ((mod & 1) && (mod < (1 << 30))) {
        using mint = Montgomery_modint_32<202311021>;
        mint::set_mod(mod);
        return mint(a).pow(n).val();
    }
    Barrett bt(mod);
    int r = 1;
    while (n) {
        if (n & 1) r = bt.mul(r, a);
        a = bt.mul(a, a), n >>= 1;
    }
    return r;
}

unsigned long long mod_pow_64(long long a, long long n, unsigned long long mod) {
    assert(n >= 0);
    if (mod == 1) return 0;
    // a %= mod with unsigned mod would convert a negative a to unsigned first
    a = ((a %= (long long)mod) < 0 ? a + mod : a);
    if ((mod & 1) && (mod < (1ULL << 62))) {
        using mint = Montgomery_modint_64<202311021>;
        mint::set_mod(mod);
        return mint(a).pow(n).val();
    }
    Barrett_64 bt(mod);
    long long r = 1;
    while (n) {
        if (n & 1) r = bt.mul(r, a);
        a = bt.mul(a, a), n >>= 1;
    }
    return r;
}
} // namespace mitsuha
#endif // AJAY_MOD_POW