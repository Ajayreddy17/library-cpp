#ifndef AJAY_PARTIAL_FRAC_DECOMPOSITION_1
#define AJAY_PARTIAL_FRAC_DECOMPOSITION_1

#include "library/poly/multipoint.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/partial_frac_decomposition_1.hpp

// f(x) / prod(1-a_ix) = sum b_i/1-a_ix
template <typename mint>
vector<mint> partial_frac_decomposition_1(vector<mint> f, vector<mint> A) {
    int N = len(A);
    assert(len(f) <= N);
    f.resize(N);
    SubproductTree<mint> X(A);
    vector<mint> g = X.T[1]; // prod(1-ax)
    g.resize(N + 1);
    reverse(f.begin(), f.end());
    reverse(g.begin(), g.end());
    For(i, len(g) - 1) g[i] = g[i + 1] * mint(i + 1);
    g.pop_back();
    auto num = X.evaluation(f);
    auto den = X.evaluation(g);
    vector<mint> B(len(A));
    For(i, len(A)) B[i] = num[i] / den[i];
    return B;
}
} // namespace mitsuha
#endif // AJAY_PARTIAL_FRAC_DECOMPOSITION_1
