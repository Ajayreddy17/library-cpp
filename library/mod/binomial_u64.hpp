#ifndef AJAY_BINOMIAL_U64
#define AJAY_BINOMIAL_U64

#include "library/mod/mod_inv_u64.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/binomial_u64.hpp
// binom(n, k) mod 2^64 for n <= LIM: odd parts of factorials and the power of 2 kept separately
struct Binomial_u64 {
    int LIM;
    vector<unsigned long long> fact, ifact, exp;
    Binomial_u64(int LIM) : LIM(LIM), fact(LIM + 1), ifact(LIM + 1), exp(LIM + 1) {
        fact[0] = 1;
        for (int i = 1; i <= LIM; ++i) {
            int k = lsb(i);
            fact[i] = fact[i - 1] * (i >> k);
            exp[i] = exp[i - 1] + k;
        }
        ifact[LIM] = mod_inv_u64(fact[LIM]);
        for (unsigned long long i = LIM; i >= 1; --i) {
            int k = lsb(i);
            ifact[i - 1] = ifact[i] * (i >> k);
        }
    }

    unsigned long long C(int n, int k) {
        assert(0 <= n && n <= LIM);
        if (k < 0 || n < k) return 0;
        int e = exp[n] - exp[k] - exp[n - k];
        if (e >= 64) return 0;
        unsigned long long x = fact[n] * ifact[k] * ifact[n - k];
        return x << e;
    }
};
} // namespace mitsuha
#endif // AJAY_BINOMIAL_U64
