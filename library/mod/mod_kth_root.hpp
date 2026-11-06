#ifndef AJAY_MOD_KTH_ROOT
#define AJAY_MOD_KTH_ROOT

#include "library/number/primetest.hpp"
#include "library/number/factor.hpp"
#include "library/mod/primitive_root.hpp"
#include "library/mod/mod_inv.hpp"
#include "library/mod/barrett.hpp"
#include "library/datastructure/hashmap.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_kth_root.hpp
// some x with x^k = a mod p (p prime), or -1. Adleman-Manders-Miller.
int mod_kth_root(long long k, long long a, int mod) {
    assert(primetest(mod) && 0 <= a && a < mod);
    if (k == 0) return (a == 1 ? 1 : -1);
    if (a == 0) return 0;
    if (mod == 2) return a;
    k %= mod - 1;
    Barrett bt(mod);

    long long g = gcd(k, mod - 1);
    if (mod_pow(a, (mod - 1) / g, mod) != 1) return -1;

    // reduce to k | mod - 1
    long long c = mod_inv(k / g, (mod - 1) / g);
    a = mod_pow(a, c, mod);
    k = (k * c) % (mod - 1);
    if (k == 0) return 1;

    g = primitive_root(mod);

    // p^e-th root of a, for p^e | k
    auto solve_pp = [&](long long p, int e, long long a) -> long long {
        int f = 0;
        long long pf = 1;
        while ((mod - 1) % (pf * p) == 0) ++f, pf *= p;
        long long m = (mod - 1) / pf;
        /*
        cyclic group of order p^f m; a has a p^e-th root.
        keep b = a / x^{p^e}; raise the number of times b's order can be divided by p.
        */
        long long x = 1, b = a, c = f - e; // b^{m p^c} = 1
        int pc = 1;
        For(c) pc *= p;
        int pe = 1;
        For(e) pe *= p;
        // baby-step giant-step table for logs to base G, a primitive p-th root of 1
        long long G = mod_pow(g, (mod - 1) / p, mod);
        int M = 0;
        HashMap<int> MP;
        long long GM_inv = -1;
        if (c) {
            while (M * M < p) ++M;
            MP.build(M);
            long long Gpow = 1;
            For(m, M) {
                MP[Gpow] = m;
                Gpow = bt.mul(Gpow, G);
            }
            GM_inv = mod_pow(Gpow, mod - 2, mod);
        }

        while (c) {
            /*
            b^{m p^c} = 1. want (b / x^{p^e})^{m p^{c-1}} = 1.
            search x = g^{p^{f-c-e} k}: reduces to B = G^k for p-th roots of unity B, G.
            */
            long long B = mod_pow(b, m * pc / p, mod);
            int k = [&](long long B) -> int {
                For(m, M + 1) {
                    if (MP.contains(B)) return m * M + MP[B];
                    B = bt.mul(B, GM_inv);
                }
                return -1;
            }(B);
            x = bt.mul(x, mod_pow(g, pf / pc / pe * k, mod));
            long long exp = pf / pc * k % (mod - 1);
            b = bt.mul(b, mod_pow(g, mod - 1 - exp, mod));
            --c;
            pc /= p;
        }
        int k = pe - mod_inv(m, pe);
        k = (k * m + 1) / pe;
        long long y = mod_pow(b, k, mod);
        x = bt.mul(x, y);
        return x;
    };

    auto pf = factor(k);
    for (auto&& [p, e] : pf) a = solve_pp(p, e, a);
    return a;
}

long long mod_kth_root_64(long long k, long long a, long long mod) {
    assert(primetest(mod) && 0 <= a && a < mod);
    if (k == 0) return (a == 1 ? 1 : -1);
    if (a == 0) return 0;
    if (mod == 2) return a;
    k %= mod - 1;

    long long g = gcd(k, mod - 1);
    if (mod_pow_64(a, (mod - 1) / g, mod) != 1) return -1;

    long long c = mod_inv(k / g, (mod - 1) / g);
    a = mod_pow_64(a, c, mod);
    k = __int128(k) * c % (mod - 1);
    if (k == 0) return 1;

    g = primitive_root_64(mod);

    auto solve_pp = [&](long long p, long long e, long long a) -> long long {
        long long f = 0;
        long long pf = 1;
        while (((mod - 1) / pf) % p == 0) ++f, pf *= p;
        long long m = (mod - 1) / pf;
        long long x = 1, b = a, c = f - e; // b^{m p^c} = 1
        long long pc = 1;
        For(c) pc *= p;
        long long pe = 1;
        For(e) pe *= p;
        long long G = mod_pow_64(g, (mod - 1) / p, mod);
        long long M = 0;
        long long GM_inv = -1;
        HashMap<long long> MP;
        if (c) {
            while (M * M < p) ++M;
            MP.build(M);
            long long Gpow = 1;
            For(m, M) {
                MP[Gpow] = m;
                Gpow = __int128(Gpow) * G % mod;
            }
            GM_inv = mod_pow_64(Gpow, mod - 2, mod);
        }

        while (c) {
            long long B = mod_pow_64(b, pc / p * m, mod);
            long long k = [&](long long B) -> long long {
                For(m, M + 1) {
                    if (MP.contains(B)) return m * M + MP[B];
                    B = __int128(B) * GM_inv % mod;
                }
                return -1;
            }(B);
            x = __int128(x) * mod_pow_64(g, pf / pc / pe * k, mod) % mod;

            long long exp = pf / pc * __int128(k) % (mod - 1);
            b = __int128(b) * mod_pow_64(g, mod - 1 - exp, mod) % mod;
            --c;
            pc /= p;
        }
        long long k = pe - mod_inv(m, pe);
        k = (__int128(k) * m + 1) / pe;
        long long y = mod_pow_64(b, k, mod);
        x = __int128(x) * y % mod;
        return x;
    };

    auto pf = factor(k);
    for (auto&& [p, e] : pf) a = solve_pp(p, e, a);
    return a;
}
} // namespace mitsuha
#endif // AJAY_MOD_KTH_ROOT
