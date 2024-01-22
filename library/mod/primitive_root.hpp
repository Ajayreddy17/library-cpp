#ifndef AJAY_PRIMITIVE_ROOT
#define AJAY_PRIMITIVE_ROOT

#include "library/number/factor.hpp"
#include "library/mod/mod_pow.hpp"
#include "library/random/base.hpp"

namespace mitsuha{
int primitive_root(int p) {
    auto pf = factor(p - 1);
    auto is_ok = [&](int g) -> bool {
        for (auto&& [q, e]: pf)
            if (mod_pow(g, (p - 1) / q, p) == 1) return false;
        return true;
    };
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
} // namespace mitsuha
#endif // AJAY_PRIMITIVE_ROOT