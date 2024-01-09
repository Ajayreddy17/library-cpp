#ifndef AJAY_FPS_EXP
#define AJAY_FPS_EXP

#include "library/poly/convolution.hpp"
#include "library/poly/integrate.hpp"
#include "library/poly/diferentiate.hpp"
#include "library/poly/count_terms.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> fps_exp_sparse(vector<mint>& f) {
    if (len(f) == 0) return {mint(1)};
    assert(f[0] == 0);
    int N = (int) f.size();
    // have df
    vector<pair<int, mint>> dat;
    for(int i = 1; i < N; i++) if (f[i] != mint(0)) dat.emplace_back(i - 1, mint(i) * f[i]);
    vector<mint> F(N);
    F[0] = 1;
    for(int n = 1; n < N; n++) {
        mint rhs = 0;
        for (auto&& [k, fk]: dat) {
            if (k > n - 1) break;
            rhs += fk * F[n - 1 - k];
        }
        F[n] = rhs * inv<mint>(n);
    }
    return F;
}

template <typename mint>
vector<mint> fps_exp_dense(vector<mint>& h) {
    const int n = (int) h.size();
    assert(n > 0 && h[0] == mint(0));
    if (mint::can_ntt()) {
        vector<mint>& f = h;
        vector<mint> b = {1, (1 < n ? f[1] : 0)};
        vector<mint> c = {1}, z1, z2 = {1, 1};
        while ((int) b.size() < n) {
            int m = (int) b.size();
            auto y = b;
            y.resize(2 * m);
            ntt(y, 0);
            z1 = z2;
            vector<mint> z(m);
            for(int i = 0; i < m; i++) z[i] = y[i] * z1[i];
            ntt(z, 1);
            for(int i = 0; i < m / 2; i++) z[i] = 0;
            ntt(z, 0);
            for(int i = 0; i < m; i++) z[i] *= -z1[i];
            ntt(z, 1);
            c.insert(c.end(), z.begin() + m / 2, z.end());
            z2 = c;
            z2.resize(2 * m);
            ntt(z2, 0);

            vector<mint> x(f.begin(), f.begin() + m);
            for(int i = 0; i < (int) x.size() - 1; i++) x[i] = x[i + 1] * mint(i + 1);
            x.back() = 0;
            ntt(x, 0);
            for(int i = 0; i < m; i++) x[i] *= y[i];
            ntt(x, 1);

            for(int i = 0; i < m - 1; i++) x[i] -= b[i + 1] * mint(i + 1);

            x.resize(m + m);
            for(int i = 0; i < m - 1; i++) x[m + i] = x[i], x[i] = 0;
            ntt(x, 0);
            for(int i = 0; i < m + m; i++) x[i] *= z2[i];
            ntt(x, 1);
            for(int i = (int) x.size() - 2; i >= 0; i--) x[i + 1] = x[i] * inv<mint>(i + 1);
            x[0] = 0;

            for(int i = m; i < min(n, m + m); i++) x[i] += f[i];
            for(int i = 0; i < m; i++) x[i] = 0;
            ntt(x, 0);
            for(int i = 0; i < m + m; i++) x[i] *= y[i];
            ntt(x, 1);
            b.insert(b.end(), x.begin() + m, x.end());
        }
        b.resize(n);
        return b;
    }

    const int L = (int) h.size();
    assert(L > 0 && h[0] == mint(0));
    int LOG = 0;
    while (1 << LOG < L) ++LOG;
    h.resize(1 << LOG);
    auto dh = differentiate(h);
    vector<mint> f = {1}, g = {1};
    int m = 1;

    vector<mint> p;

    for(int _ = 0; _ < LOG; _++) {
        p = convolution(f, g);
        p.resize(m);
        p = convolution(p, g);
        p.resize(m);
        g.resize(m);
        for(int i = 0; i < m; i++) g[i] += g[i] - p[i];
        p = {dh.begin(), dh.begin() + m - 1};
        p = convolution(f, p);
        p.resize(m + m - 1);
        for(int i = 0; i < m + m - 1; i++) p[i] = -p[i];
        for(int i = 0; i < m - 1; i++) p[i] += mint(i + 1) * f[i + 1];
        p = convolution(p, g);

        p.resize(m + m - 1);
        for(int i = 0; i < m - 1; i++) p[i] += dh[i];
        p = integrate(p);
        for(int i = 0; i < m + m; i++) p[i] = h[i] - p[i];
        p[0] += mint(1);
        f = convolution(f, p);
        f.resize(m + m);
        m += m;
    }
    f.resize(L);
    return f;
}

template <typename mint>
vector<mint> fps_exp(vector<mint>& f) {
    int n = count_terms(f);
    int t = (mint::can_ntt() ? 320 : 3000);
    return (n <= t ? fps_exp_sparse<mint>(f) : fps_exp_dense<mint>(f));
}
} // namespace mitsuha
#endif // AJAY_FPS_EXP