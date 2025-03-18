#ifndef AJAY_SUM_OF_C_NEGATIVE
#define AJAY_SUM_OF_C_NEGATIVE

#include "library/datastructure/sliding_window_aggregation.hpp"
#include "library/monoid/monoid_mul.hpp"

namespace mitsuha {
// calculate [x^N] f(x)(1-x)^{-K} in O(deg(f)+K).
template <typename mint>
mint sum_of_C_negative(long long N, long long K, vector<mint>& f) {
    assert(K >= 0);
    if (N < 0) return mint(1);
    if (K == 0) return (N < len(f) ? f[N] : mint(0));
    K -= 1;
    Sliding_Window_Aggregation<Monoid_Mul<mint>> seg;
    For(i, K) seg.push(N + K - i);
    mint ANS = 0;
    For(i, len(f)) {
        ANS += f[i] * seg.prod();
        seg.push(N - i);
        seg.pop();
    }
    return ANS * fact_inv<mint>(K);
}
} // namespace mitsuha
#endif // AJAY_SUM_OF_C_NEGATIVE