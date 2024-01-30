#ifndef AJAY_MOD_SQRT
#define AJAY_MOD_SQRT

#include "library/random/base.hpp"
#include "library/mod/mod_pow.hpp"

namespace mitsuha{
// p is a prime number. No solution is -1.
int mod_sqrt(int a, int p) {
    if (p == 2) return a;
    if (a == 0) return 0;
    int k = (p - 1) / 2;
    if (mod_pow(a, k, p) != 1) return -1;
    auto find = [&]() -> pair<long long, long long> {
        while (1) {
            long long b = RNG(2, p);
            long long D = (b * b - a) % p;
            if (D == 0) return {b, D};
            if (mod_pow(D, k, p) != 1) return {b, D};
        }
    };
    auto [b, D] = find();
    if (D == 0) return b;
    ++k;
    // (b + sqrt(D))^k
    long long f0 = b, f1 = 1, g0 = 1, g1 = 0;
    while (k) {
        if (k & 1) {
            tie(g0, g1) = make_pair(f0 * g0 + D * f1 % p * g1, f1 * g0 + f0 * g1);
            g0 %= p, g1 %= p;
        }
        tie(f0, f1) = make_pair(f0 * f0 + D * f1 % p * f1, 2 * f0 * f1);
        f0 %= p, f1 %= p;
        k >>= 1;
    }
    if (g0 < 0) g0 += p;
    return g0;
}

// p is a prime number. No solution is -1.
long long mod_sqrt_64(long long a, long long p) {
    if (p == 2) return a;
    if (a == 0) return 0;
    long long k = (p - 1) / 2;
    if (mod_pow_64(a, k, p) != 1) return -1;
    auto find = [&]() -> pair<__int128, __int128> {
        while (1) {
            __int128 b = RNG(2, p);
            __int128 D = b * b - a;
            if (D == 0) return {b, D};
            if (mod_pow_64(D, k, p) != 1) return {b, D};
        }
    };
    auto [b, D] = find();
    if (D == 0) return b;
    ++k;
    // (b + sqrt(D))^k
    __int128 f0 = b, f1 = 1, g0 = 1, g1 = 0;
    while (k) {
        if (k & 1) {
            tie(g0, g1) = make_pair(f0 * g0 + D * f1 % p * g1, f1 * g0 + f0 * g1);
            g0 %= p, g1 %= p;
        }
        tie(f0, f1) = make_pair(f0 * f0 + D * f1 % p * f1, 2 * f0 * f1);
        f0 %= p, f1 %= p;
        k >>= 1;
    }
    return g0;
}
} // namespace mitsuha
#endif // AJAY_MOD_SQRT