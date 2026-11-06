#ifndef AJAY_FIRST_MOD_RANGE_OF_LINEAR
#define AJAY_FIRST_MOD_RANGE_OF_LINEAR

#include "library/mod/min_of_linear_segments.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/first_mod_range_of_linear.hpp
// smallest x >= 0 with (ax + b) mod mod in {L, ..., R - 1} (taken mod mod), or -1
// L < 0 and R >= mod are allowed: e.g. ax + b = -1 mod m is the range [-1, 0)
int first_mod_range_of_linear(long long L, long long R, long long a, long long b, int mod) {
    assert(L <= R);
    b -= L, R -= L;
    if (R >= mod) return 0;
    a = rem<long long>(a, mod), b = rem<long long>(b, mod);
    // ax + b < R
    if (b < R) return 0;
    auto [X, DX] = min_of_linear_segments(a, b, mod);
    For(i, len(DX)) {
        long long x1 = X[i], x2 = X[i + 1];
        long long y2 = (a * x2 + b) % mod;
        if (y2 >= R) continue;
        long long y1 = (a * x1 + b) % mod;
        long long d = (y1 - y2) * DX[i] / (x2 - x1);
        long long k = fld(y1 - R, d) + 1;
        return x1 + k * DX[i];
    }
    return -1;
}
} // namespace mitsuha
#endif // AJAY_FIRST_MOD_RANGE_OF_LINEAR
