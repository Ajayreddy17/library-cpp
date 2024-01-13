#ifndef AJAY_PRIMETEST
#define AJAY_PRIMETEST

#include "library/mod/mongomery_modint.hpp"

namespace mitsuha{
bool primetest(const unsigned long long x) {
    assert(x < 1ULL << 62);
    if (x == 2 or x == 3 or x == 5 or x == 7) return true;
    if (x % 2 == 0 or x % 3 == 0 or x % 5 == 0 or x % 7 == 0) return false;
    if (x < 121) return x > 1;
    const unsigned long long d = (x - 1) >> ((x - 1) == 0 ? -1 : __builtin_ctzll(x - 1));

    using mint = Mongomery_modint_64<202311020>;

    mint::set_mod(x);
    const mint one(1ULL), minus_one(x - 1);
    auto ok = [&](unsigned long long a) -> bool {
        auto y = mint(a).pow(d);
        unsigned long long t = d;
        while (y != one && y != minus_one && t != x - 1) y *= y, t <<= 1;
        if (y != minus_one && t % 2 == 0) return false;
        return true;
    };
    if (x < (1ULL << 32)) {
        for (unsigned long long a: {2, 7, 61})
            if (!ok(a)) return false;
    } else {
        for (unsigned long long a: {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
            if (!ok(a)) return false;
        }
    }
    return true;
}
} // namespace mitsuha
#endif // AJAY_PRIMETEST