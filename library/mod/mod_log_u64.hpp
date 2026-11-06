#ifndef AJAY_MOD_LOG_U64
#define AJAY_MOD_LOG_U64

#include "library/mod/mod_inv_u64.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_log_u64.hpp
// odd a = (-1)^sgn * 5^K mod 2^64, returns {sgn, K mod 2^62}
// https://contest.ucup.ac/contest/2693/problem/15032
pair<unsigned long long, unsigned long long> mod_log_u64_primitive_root(unsigned long long a) {
    assert(a & 1);
    unsigned long long sgn = 0;
    if (a & 2) sgn = 1, a = -a;
    // X = 5^K mod 2^n
    long long n = 2;
    unsigned long long K = 0;
    unsigned long long X = 1;
    unsigned long long Y = 5; // pow(5, 2^{n-2})
    For(62) {
        if ((X - a) >> n & 1) {
            K += 1ULL << (n - 2);
            X *= Y;
        }
        Y *= Y;
        ++n;
    }
    return {sgn, K};
}

// smallest n >= 0 with a^n = b mod 2^64, or -1
long long mod_log_u64(unsigned long long a, unsigned long long b) {
    if (a % 2 == 0) {
        unsigned long long x = 1;
        For(n, 65) {
            if (x == b) return n;
            x *= a;
        }
    }
    if (a * b % 2 == 0) return -1;

    auto [X1, Y1] = mod_log_u64_primitive_root(a);
    auto [X2, Y2] = mod_log_u64_primitive_root(b);
    // X1 * n = X2 mod 2, Y1 * n = Y2 mod 2^62
    unsigned long long mod = 1ULL << 62;
    long long k = lsb(Y1);
    k = (k == -1 ? 62 : k);
    if (Y2 & ((1ULL << k) - 1)) return -1;
    Y1 >>= k;
    Y2 >>= k;
    mod >>= k;
    unsigned long long n = (mod == 1 ? 0 : mod_inv_u64(Y1) * Y2 % mod);

    For(2) {
        if (X1 * n % 2 == X2) return n;
        n += mod;
    }
    return -1;
}
} // namespace mitsuha
#endif // AJAY_MOD_LOG_U64
