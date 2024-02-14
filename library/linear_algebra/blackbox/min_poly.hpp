#ifndef AJAY_BLACKBOX_MIN_POLY
#define AJAY_BLACKBOX_MIN_POLY

#include "library/seq/find_linear_rec.hpp"
#include "library/random/base.hpp"

namespace mitsuha{
// Pass the linear transformation f representing the multiplication of matrix A
// auto f = [&](vc<mint> v) -> vc<mint> {};
template <typename mint, typename F>
vector<mint> blackbox_min_poly(int N, F f) {
    vector<mint> S(N + N + 10);
    vector<mint> c(N);
    vector<mint> v(N);
    For(i, N) c[i] = RNG(0, mint::get_mod());
    For(i, N) v[i] = RNG(0, mint::get_mod());
    For(k, N + N + 10) {
        For(i, N) S[k] += c[i] * v[i];
        v = f(v);
    }
    S = find_linear_rec(S);
    reverse(S.begin(), S.end());
    return S;
}
} // namespace mitsuha
#endif // AJAY_BLACKBOX_MIN_POLY