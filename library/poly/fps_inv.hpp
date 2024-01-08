#ifndef AJAY_FPS_INV
#define AJAY_FPS_INV

#include "library/poly/convolution.hpp"
#include "library/poly/count_terms.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> fps_inv_sparse(const vector<mint>& f) {
    int N = (int) f.size();
    vector<pair<int, mint>> dat;
    for(int i = 1; i < N; i++) if (f[i] != mint(0)) dat.emplace_back(i, f[i]);
    vector<mint> g(N);
    mint g0 = mint(1) / f[0];
    g[0] = g0;
    for(int n = 1; n < N; n++) {
        mint rhs = 0;
        for (auto&& [k, fk]: dat) {
            if (k > n) break;
            rhs -= fk * g[n - k];
        }
        g[n] = rhs * g0;
    }
    return g;
}

template <typename mint>
vector<mint> fps_inv_dense_ntt(const vector<mint>& F) {
    vector<mint> G = {mint(1) / F[0]};
    int N = (int) F.size(), n = 1;
    G.reserve(N);
    while (n < N) {
        vector<mint> f(2 * n), g(2 * n);
        for(int i = 0; i < min(N, 2 * n); i++) f[i] = F[i];
        for(int i = 0; i < n; i++) g[i] = G[i];
        ntt(f, false), ntt(g, false);
        for(int i = 0; i < 2 * n; i++) f[i] *= g[i];
        ntt(f, true);
        for(int i = 0; i < n; i++) f[i] = 0;
        ntt(f, false);
        for(int i = 0; i < 2 * n; i++) f[i] *= g[i];
        ntt(f, true);
        for(int i = n; i < min(N, 2 * n); i++) G.emplace_back(-f[i]);
        n *= 2;
    }
    return G;
}

template <typename mint>
vector<mint> fps_inv_dense(const vector<mint>& F) {
    if (mint::can_ntt()) return fps_inv_dense_ntt(F);
    const int N = (int) F.size();
    vector<mint> R = {mint(1) / F[0]};
    vector<mint> p;
    int m = 1;
    while (m < N) {
        p = convolution(R, R);
        p.resize(m + m);
        vector<mint> f = {F.begin(), F.begin() + min(m + m, N)};
        p = convolution(p, f);
        R.resize(m + m);
        for(int i = 0; i < m + m; i++) R[i] = R[i] + R[i] - p[i];
        m += m;
    }
    R.resize(N);
    return R;
}

template <typename mint>
vector<mint> fps_inv(const vector<mint>& f) {
    assert(f[0] != mint(0));
    int n = count_terms(f);
    int t = (mint::can_ntt() ? 160 : 820);
    return (n <= t ? fps_inv_sparse<mint>(f) : fps_inv_dense<mint>(f));
}
} // namespace mitsuha
#endif // AJAY_FPS_INV