#ifndef AJAY_FPS_LOG
#define AJAY_FPS_LOG

#include "library/poly/count_terms.hpp"
#include "library/poly/fps_inv.hpp"

namespace mitsuha{
template <typename mint>
vector<mint> fps_log_dense(const vector<mint>& f) {
    assert(f[0] == mint(1));
    int N = (int) f.size();
    vector<mint> df = f;
    for(int i = 0; i < N; i++) df[i] *= mint(i);
    df.erase(df.begin());
    auto f_inv = fps_inv(f);
    auto g = convolution(df, f_inv);
    g.resize(N - 1);
    g.insert(g.begin(), 0);
    for(int i = 0; i < N; i++) g[i] *= inv<mint>(i);
    return g;
}

template <typename mint>
vector<mint> fps_log_sparse(const vector<mint>& f) {
    int N = f.size();
    vector<pair<int, mint>> dat;
    for(int i = 1; i < N; i++) if (f[i] != mint(0)) dat.emplace_back(i, f[i]);
    vector<mint> F(N);
    vector<mint> g(N - 1);
    for (int n = 0; n < N - 1; ++n) {
        mint rhs = mint(n + 1) * f[n + 1];
        for (auto&& [i, fi]: dat) {
            if (i > n) break;
            rhs -= fi * g[n - i];
        }
        g[n] = rhs;
        F[n + 1] = rhs * inv<mint>(n + 1);
    }
    return F;
}

template <typename mint>
vector<mint> fps_log(const vector<mint>& f) {
    assert(f[0] == mint(1));
    int n = count_terms(f);
    int t = (mint::can_ntt() ? 200 : 1200);
    return (n <= t ? fps_log_sparse<mint>(f) : fps_log_dense<mint>(f));
}
} // namespace mitsuha
#endif // AJAY_FPS_LOG