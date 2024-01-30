#ifndef AJAY_FPS_SQRT
#define AJAY_FPS_SQRT

#include "library/poly/fps_inv.hpp"
#include "library/poly/fps_pow.hpp"
#include "library/poly/count_terms.hpp"
#include "library/mod/mod_sqrt.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> fps_sqrt_dense(vector<mint>& f) {
    assert(f[0] == mint(1));
    int n = len(f);
    vector<mint> R = {1};
    while (len(R) < n) {
        int m = min(2 * int(len(R)), n);
        R.resize(m);
        vector<mint> tmp = {f.begin(), f.begin() + m};
        tmp = convolution(tmp, fps_inv(R));
        tmp.resize(m);
        For(i, m) R[i] += tmp[i];
        mint c = mint(1) / mint(2);
        For(i, len(R)) R[i] *= c;
    }
    R.resize(n);
    return R;
}

template <typename mint>
vector<mint> fps_sqrt_sparse(vector<mint>& f) {
    return fps_pow_1_sparse(f, inv<mint>(2));
}

template <typename mint>
vector<mint> fps_sqrt(vector<mint>& f) {
    if (count_terms(f) <= 200) return fps_sqrt_sparse(f);
    return fps_sqrt_dense(f);
}

template <typename mint>
vector<mint> fps_sqrt_any(vector<mint>& f) {
    int n = len(f);
    int d = n;
    Frr(i, n) if (f[i] != 0) d = i;
    if (d == n) return f;
    if (d & 1) return {};
    mint y = f[d];
    mint x = mod_sqrt(y.val, mint::get_mod());
    if (x * x != y) return {};
    mint c = mint(1) / y;
    vector<mint> g(n - d);
    For(i, n - d) g[i] = f[d + i] * c;
    g = fps_sqrt(g);
    For(i, len(g)) g[i] *= x;
    g.resize(n);
    Frr(i, n) {
        if (i >= d / 2)
            g[i] = g[i - d / 2];
        else
            g[i] = 0;
    }
    return g;
}
} // namepsace mitsuha
#endif // AJAY_FPS_SQRT
