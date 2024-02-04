#ifndef AJAY_POWERTABLE
#define AJAY_POWERTABLE

#include "library/number/primetable.hpp"

namespace mitsuha{
// a^0, ..., a^N
template <typename mint>
vector<mint> powertable_1(mint a, long long N) {
    // table of a^i
    vector<mint> f(N + 1, 1);
    For(i, N) f[i + 1] = a * f[i];
    return f;
}

// 0^e, ..., N^e
template <typename mint>
vector<mint> powertable_2(long long e, long long N) {
    auto primes = primetable(N);
    vector<mint> f(N + 1, 1);
    f[0] = mint(0).pow(e);
    for (auto&& p: primes) {
        if (p > N) break;
        mint xp = mint(p).pow(e);
        long long pp = p;
        while (pp <= N) {
            long long i = pp;
            while (i <= N) {
                f[i] *= xp;
                i += pp;
            }
            pp *= p;
        }
    }
    return f;
}
} // namespace mitsuha
#endif // AJAY_POWERTABLE