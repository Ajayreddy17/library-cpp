#ifndef AJAY_MIN_OF_LINEAR
#define AJAY_MIN_OF_LINEAR

#include "library/mod/min_of_linear_segments.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/min_of_linear.hpp
// min_{x in [L, R)} (ax + b mod mod), returns {argmin x (smallest), min value}
pair<long long, int> min_of_linear(long long L, long long R, int a, int b, int mod) {
    assert(L < R);
    a %= mod;
    if (a < 0) a += mod;
    long long n = R - L;
    b = (b + (long long)a * rem<long long>(L, mod)) % mod;
    if (b < 0) b += mod;
    auto [X, DX] = min_of_linear_segments(a, b, mod);
    long long x = 0;
    for (int i = 0; i < int(X.size()) - 1; ++i) {
        long long xl = X[i], xr = X[i + 1];
        if (xr < n) {
            x = xr;
            continue;
        }
        x = xl + (n - 1 - x) / DX[i] * DX[i];
        break;
    }
    int y = ((long long)a * x + b) % mod;
    return {L + x, y};
}
} // namespace mitsuha
#endif // AJAY_MIN_OF_LINEAR
