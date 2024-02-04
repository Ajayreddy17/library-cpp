#ifndef AJAY_POLY_TAYLOR_SHIFT
#define AJAY_POLY_TAYLOR_SHIFT

#include "library/mod/powertable.hpp"
#include "library/poly/convolution.hpp"

namespace mitsuha{
// f(x) -> f(x+c)
template <typename mint>
vector<mint> poly_taylor_shift(vector<mint> f, mint c) {
    long long N = len(f);
    For(i, N) f[i] *= fact<mint>(i);
    auto b = powertable_1<mint>(c, N);
    For(i, N) b[i] *= fact_inv<mint>(i);
    reverse(f.begin(), f.end());
    f = convolution(f, b);
    f.resize(N);
    reverse(f.begin(), f.end());
    For(i, N) f[i] *= fact_inv<mint>(i);
    return f;
}
} // namespace mitsuha
#endif // AJAY_POLY_TAYLOR_SHIFT