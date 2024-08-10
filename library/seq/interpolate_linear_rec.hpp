#ifndef AJAY_INTERPOLATE_LINEAR_REC
#define AJAY_INTERPOLATE_LINEAR_REC

#include "library/poly/coef_of_rational_fps.hpp"
#include "library/seq/find_linear_rec.hpp"

namespace mitsuha{
template <typename mint>
mint interpolate_linear_rec(vector<mint> A, long long N, int off) {
    if (N < len(A)) return A[N];
    A = {A.begin() + off, A.end()};
    N -= off;
    auto G = find_linear_rec(A);
    auto F = convolution(A, G);
    F.resize(len(G) - 1);
    return coef_of_rational_fps(F, G, N);
}
} // namespace mitsuha
#endif // AJAY_INTERPOLATE_LINEAR_REC