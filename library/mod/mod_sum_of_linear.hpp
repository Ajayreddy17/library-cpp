#ifndef AJAY_MOD_SUM_OF_LINEAR
#define AJAY_MOD_SUM_OF_LINEAR

#include "library/mod/floor_sum_of_linear.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/mod_sum_of_linear.hpp
// sum_{x in [L, R)} ((ax + b) mod mod)
__int128 mod_sum_of_linear(long long L, long long R, long long a, long long b, long long mod) {
    __int128 s = (__int128)a * L + b;
    __int128 t = (__int128)a * (R - 1) + b;
    __int128 sum = (s + t) * (R - L) / 2;
    __int128 fsum = floor_sum_of_linear(L, R, a, b, mod);
    return sum - fsum * mod;
}
} // namespace mitsuha
#endif // AJAY_MOD_SUM_OF_LINEAR
