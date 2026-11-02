#ifndef AJAY_MIN_COST_MATCHING_ON_LINE
#define AJAY_MIN_COST_MATCHING_ON_LINE

#include "library/convex/slope_trick/slope_trick_0.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/min_cost_matching_on_line.hpp

// A[i] items at position i, position i accepts up to B[i] items; cost = |distance|
long long min_cost_matching_on_line(vector<long long> A, vector<long long> B) {
    assert(accumulate(A.begin(), A.end(), 0LL) <= accumulate(B.begin(), B.end(), 0LL));
    const int N = len(A);
    Slope_Trick_0 f;
    For(N + 1) f.add_left(0);
    For(i, N) {
        long long c = A[i] - B[i];
        f.shift(c);
        f.clear_right();
        f.add_abs(0);
    }
    return (long long)f.eval(0);
}
} // namespace mitsuha
#endif // AJAY_MIN_COST_MATCHING_ON_LINE
