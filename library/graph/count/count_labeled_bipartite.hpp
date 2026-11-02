#ifndef AJAY_COUNT_LABELED_BIPARTITE
#define AJAY_COUNT_LABELED_BIPARTITE

#include "library/poly/fps_log.hpp"
#include "library/poly/fps_sqrt.hpp"
#include "library/datastructure/power_query.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_bipartite.hpp

// connected = false: https://oeis.org/A047864
// connected = true: https://oeis.org/A001832
template <typename mint>
vector<mint> count_labeled_bipartite(int N, bool connected) {
    // colored bipartite
    vector<mint> F(N + 1);
    mint ipow = 1;
    F[0] = 1;
    For(i, 1, N + 1) F[i] = F[i - 1] * ipow, ipow *= inv<mint>(2);
    For(i, N + 1) F[i] *= fact_inv<mint>(i);
    F = convolution(F, F);
    F.resize(N + 1);
    mint pow = 1, c = 1;
    For(i, N + 1) F[i] *= c, c *= pow, pow += pow;

    if (connected) {
        F = fps_log(F);
        For(i, N + 1) F[i] *= inv<mint>(2);
        For(i, N + 1) F[i] *= fact<mint>(i);
        return F;
    }
    F = fps_sqrt(F);
    For(i, N + 1) F[i] *= fact<mint>(i);
    return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_BIPARTITE
