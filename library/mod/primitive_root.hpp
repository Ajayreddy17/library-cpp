#ifndef AJAY_PRIMITIVE_ROOT
#define AJAY_PRIMITIVE_ROOT

#include "library/number/factor.hpp"
#include "library/mod/mod_pow.hpp"
#include "library/random/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/primitive_root.hpp
// min = true: smallest primitive root, otherwise a random one
int primitive_root(int p, bool min = true) {
    auto pf = factor(p - 1);
    auto is_ok = [&](int g) -> bool {
        for (auto&& [q, e]: pf)
            if (mod_pow(g, (p - 1) / q, p) == 1) return false;
        return true;
    };
    if (min) {
        For(x, 1, p) if (is_ok(x)) return x;
    }
    while (1) {
        int x = RNG(1, p);
        if (is_ok(x)) return x;
    }
    return -1;
}

long long primitive_root_64(long long p) {
    auto pf = factor(p - 1);
    auto is_ok = [&](long long g) -> bool {
        for (auto&& [q, e]: pf)
            if (mod_pow_64(g, (p - 1) / q, p) == 1) return false;
        return true;
    };
    while (1) {
        long long x = RNG(1, p);
        if (is_ok(x)) return x;
    }
    return -1;
}

// primitive root mod p^e for an odd prime p
// https://codeforces.com/contest/1190/problem/F
long long primitive_root_prime_power_64(long long p, long long e) {
    assert(p >= 3);
    long long g = primitive_root_64(p);
    long long q = p;
    long long phi = p - 1;
    For(e - 1) {
        q *= p;
        phi *= p;
        if (mod_pow_64(g, phi / p, q) == 1) g += q / p;
    }
    return g;
}
} // namespace mitsuha
#endif // AJAY_PRIMITIVE_ROOT