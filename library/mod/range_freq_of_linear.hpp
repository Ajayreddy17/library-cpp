#ifndef AJAY_RANGE_FREQ_OF_LINEAR
#define AJAY_RANGE_FREQ_OF_LINEAR

#include "library/mod/floor_sum_of_linear.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/range_freq_of_linear.hpp
// count x in [L, R) such that lo <= (ax + b) mod mod < hi
// I: ax + b must not overflow in the range
template <typename O = __int128, typename I = long long>
I range_freq_of_linear(I L, I R, I a, I b, I mod, I lo, I hi) {
    if (lo >= hi) return 0;
    assert(0 <= lo && lo < hi && hi <= mod);
    O x1 = floor_sum_of_linear<O, I>(L, R, a, b - lo, mod);
    O x2 = floor_sum_of_linear<O, I>(L, R, a, b - hi, mod);
    return x1 - x2;
}
} // namespace mitsuha
#endif // AJAY_RANGE_FREQ_OF_LINEAR
