#ifndef AJAY_COUNT_LABELED_DAG
#define AJAY_COUNT_LABELED_DAG

#include "library/poly/fps_inv.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_dag.hpp

// https://oeis.org/A003024
template <typename mint>
vector<mint> count_labeled_dag(int N) {
    vector<mint> F(N + 1, 1);
    mint pow = 1;
    For(n, 1, N + 1) F[n] = F[n - 1] * pow, pow *= inv<mint>(2);
    For(i, N + 1) F[i] *= fact_inv<mint>(i);
    For(i, N + 1) if (i % 2 == 1) F[i] = -F[i];

    F = fps_inv(F);
    For(i, N + 1) F[i] *= fact<mint>(i);
    mint c = 1;
    pow = 1;
    For(i, N + 1) F[i] *= c, c *= pow, pow += pow;
    return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_DAG
