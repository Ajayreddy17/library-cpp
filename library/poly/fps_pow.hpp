#ifndef AJAY_FPS_POW
#define AJAY_FPS_POW

#include "library/poly/count_terms.hpp"
#include "library/poly/fps_exp.hpp"
#include "library/poly/fps_log.hpp"

namespace mitsuha{
// Find fps to the kth power. The assumption is that k >= 0.
// If the constant term is 1 and k is mint, use fps_pow_1.
// ・Dense case: use log, exp O(NlogN)
// ・If sparse: O(NK)
template <typename mint>
vector<mint> fps_pow(const vector<mint>& f, long long k) {
    assert(0 <= k);
    int n = len(f);
    if (k == 0) {
        vector<mint> g(n);
        g[0] = mint(1);
        return g;
    }
    int d = n;
    for(int i = n - 1; i >= 0; --i) if (f[i] != 0) d = i;
    // d * k >= n
    if (d >= cld<long long>(n, k)) {
        vector<mint> g(n);
        return g;
    }
    long long off = d * k;
    mint c = f[d];
    mint c_inv = mint(1) / mint(c);
    vector<mint> g(n - off);
    For(i, n - off) g[i] = f[d + i] * c_inv;
    g = fps_pow_1(g, mint(k));
    vector<mint> h(n);
    c = c.pow(k);
    For(i, len(g)) h[off + i] = g[i] * c;
    return h;
}

template <typename mint>
vector<mint> fps_pow_1_sparse(const vector<mint>& f, mint K) {
    int N = len(f);
    assert(N == 0 || f[0] == mint(1));
    vector<pair<int, mint>> dat;
    For(i, 1, N) if (f[i] != mint(0)) dat.emplace_back(i, f[i]);
    vector<mint> g(N);
    g[0] = 1;
    For(n, N - 1) {
        mint& x = g[n + 1];
        for (auto&& [d, cf]: dat) {
            if (d > n + 1) break;
            mint t = cf * g[n - d + 1];
            x += t * (K * mint(d) - mint(n - d + 1));
        }
        x *= inv<mint>(n + 1);
    }
    return g;
}

template <typename mint>
vector<mint> fps_pow_1_dense(const vector<mint>& f, mint K) {
    assert(f[0] == mint(1));
    auto log_f = fps_log(f);
    For(i, len(f)) log_f[i] *= K;
    return fps_exp_dense(log_f);
}

template <typename mint>
vector<mint> fps_pow_1(const vector<mint>& f, mint K) {
    int n = count_terms(f);
    int t = (mint::can_ntt() ? 100 : 1300);
    return (n <= t ? fps_pow_1_sparse(f, K) : fps_pow_1_dense(f, K));
}

// f^e, sparse, O(NMK)
template <typename mint>
vector<vector<mint>> fps_pow_1_sparse_2d(vector<vector<mint>> f, mint n) {
    assert(f[0][0] == mint(1));
    int N = len(f), M = len(f[0]);
    vector<vector<mint>> dp(N, vector<mint>(M));
    dp[0] = fps_pow_1_sparse<mint>(f[0], n);

    vector<tuple<int, int, mint>> dat;
    For(i, N) For(j, M) {
        if ((i > 0 || j > 0) && f[i][j] != mint(0)) dat.emplace_back(i, j, f[i][j]);
    }
    For(i, 1, N) {
        For(j, M) {
            // F = f^n, f dF = n df F
            // [x^{i-1}y^j]
            mint lhs = 0, rhs = 0;
            for (auto&& [a, b, c]: dat) {
                if (a < i && b <= j) lhs += dp[i - a][j - b] * mint(i - a);
                if (a <= i && b <= j) rhs += dp[i - a][j - b] * c * mint(a);
            }
            dp[i][j] = (n * rhs - lhs) * inv<mint>(i);
        }
    }
    return dp;
}
} // namespace mitsuha
#endif // AJAY_FPS_POW