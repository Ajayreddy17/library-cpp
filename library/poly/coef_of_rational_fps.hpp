#ifndef AJAY_COEF_OF_RATIONAL_FPS
#define AJAY_COEF_OF_RATIONAL_FPS

#include "library/poly/fps_div.hpp"
#include "library/poly/ntt_doubling.hpp"
#include "library/poly/poly_divmod.hpp"

namespace mitsuha{
template <typename mint>
mint coef_of_rational_fps_small(vector<mint> P, vector<mint> Q, long long N) {
    assert(0 <= len(P) && len(P) + 1 == len(Q) && len(Q) <= 16
           && Q[0] == mint(1));
    if (P.empty()) return 0;
    int m = len(Q) - 1;
    vector<unsigned int> Q32(m + 1);
    For(i, m + 1) Q32[i] = (-Q[i]).val;

    using poly = vector<unsigned long long>;
    auto dfs = [&](auto& dfs, const long long N) -> poly {
        // x^N mod G
        if (N == 0) {
            poly f(m);
            f[0] = 1;
            return f;
        }
        poly f = dfs(dfs, N / 2);
        poly g(len(f) * 2 - 1 + (N & 1));
        For(i, len(f)) For(j, len(f)) { g[i + j + (N & 1)] += f[i] * f[j]; }
        For(i, len(g)) g[i] = mint(g[i]).val;
        Frr(i, len(g)) {
            g[i] = mint(g[i]).val;
            if (i >= m) For(j, 1, len(Q)) g[i - j] += Q32[j] * g[i];
        }
        g.resize(m);
        return g;
    };
    poly f = dfs(dfs, N);
    For(i, m) For(j, 1, i + 1) { P[i] -= Q[j] * P[i - j]; }
    unsigned long long res = 0;
    For(i, m) res += f[i] * P[i].val;
    return res;
}

template <typename mint>
mint coef_of_rational_fps_ntt(vector<mint> P, vector<mint> Q, long long N) {
    assert(0 <= len(P) && len(P) + 1 == len(Q) && Q[0] == mint(1));
    if (P.empty()) return 0;

    int n = 1;
    while (n < len(Q)) n += n;

    vector<mint> W(n);
    {
        vector<int> btr(n);
        int log = (n == 0 ? -1 : 31 - __builtin_clz(n));
        For(i, n) { btr[i] = (btr[i >> 1] >> 1) + ((i & 1) << (log - 1)); }
        int t = mint::ntt_info().first;
        mint r = mint::ntt_info().second;
        mint dw = r.inverse().pow((1 << t) / (2 * n));
        mint w = inv<mint>(2);
        for (auto& i: btr) { W[i] = w, w *= dw; }
    }

    P.resize(2 * n), Q.resize(2 * n);
    ntt(P, 0), ntt(Q, 0);

    while (N >= n) {
        if (N % 2 == 0) For(i, n) 
                P[i] = (P[2 * i] * Q[2 * i + 1] + P[2 * i + 1] * Q[2 * i]) * inv<mint>(2);
        else For(i, n)
                P[i] = (P[2 * i] * Q[2 * i + 1] - P[2 * i + 1] * Q[2 * i]) * W[i];
        For(i, n) Q[i] = Q[2 * i] * Q[2 * i + 1];
        P.resize(n), Q.resize(n);
        N /= 2;
        if (N < n) break;
        ntt_doubling(P), ntt_doubling(Q);
    }
    ntt(P, 1), ntt(Q, 1);
    Q = fps_inv<mint>(Q);
    mint ans = 0;
    For(i, N + 1) ans += P[i] * Q[N - i];
    return ans;
}

template <typename mint>
mint coef_of_rational_fps_convolution(vector<mint> P, vector<mint> Q, long long N) {
    assert(0 <= len(P) && len(P) + 1 == len(Q) && Q[0] == mint(1));
    if (P.empty()) return 0;
    while (N >= len(P)) {
        vector<mint> Q1 = Q;
        For(i, len(Q1)) if (i & 1) Q1[i] = -Q1[i];
        P = convolution(P, Q1);
        Q = convolution(Q, Q1);
        For(i, len(Q1)) Q[i] = Q[2 * i];
        For(i, len(Q1) - 1) P[i] = P[2 * i | (N & 1)];
        P.resize(len(Q1) - 1);
        Q.resize(len(Q1));
        N /= 2;
    }
    return fps_div(P, Q)[N];
}

template <typename mint>
mint coef_of_rational_fps(vector<mint> P, vector<mint> Q, long long N) {
    if (P.empty()) return 0;
    assert(len(Q) > 0 && Q[0] != mint(0));
    while (Q.back() == mint(0)) Q.back(), Q.pop_back();
    mint c = mint(1) / Q[0];
    for (auto& x: P) x *= c;
    for (auto& x: Q) x *= c;
    mint base = 0;
    if (len(P) >= len(Q)) {
        auto [f, g] = poly_divmod<mint>(P, Q);
        base = (N < len(f) ? f[N] : mint(0));
        P = g;
    }
    P.resize(len(Q) - 1);
    int n = len(Q);
    if (mint::ntt_info().first != -1) {
        if (n <= 10) return base + coef_of_rational_fps_small(P, Q, N);
        if (n > 10) return base + coef_of_rational_fps_ntt(P, Q, N);
    }
    mint x = (n <= 16 ? coef_of_rational_fps_small(P, Q, N)
                      : coef_of_rational_fps_convolution(P, Q, N));
    return base + x;
}
} // namespace mitsuha
#endif // AJAY_COEF_OF_RATIONAL_FPS