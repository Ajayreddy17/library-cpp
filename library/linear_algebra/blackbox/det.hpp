#ifndef AJAY_BLACKBOX_DET
#define AJAY_BLACKBOX_DET

#include "library/linear_algebra/blackbox/min_poly.hpp"

namespace mitsuha{
template <typename mint, typename F>
mint blackbox_det(int N, F apply) {
    vector<mint> c(N);
    For(i, N) c[i] = RNG(1, mint::get_mod());
    mint r = 1;
    For(i, N) r *= c[i];
    auto g = [&](vector<mint> v) -> vector<mint> {
        For(i, N) v[i] *= c[i];
        return apply(v);
    };

    auto f = blackbox_min_poly<mint>(N, g);
    mint det = (len(f) == N + 1 ? f[0] : mint(0));
    if (N & 1) det *= -1;
    det /= r;
    return det;
}
} // namespace mitsuha
#endif // AJAY_BLACKBOX_DET