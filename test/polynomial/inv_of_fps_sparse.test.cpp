#define PROBLEM \
    "https://judge.yosupo.jp/problem/inv_of_formal_power_series_sparse"
#include <library/template.hpp>

#include <library/poly/fps_inv.hpp>

void solve() {
    using mint = modint998;
    LL(N, K);
    vector<mint> f(N);
    For(K) {
        LL(i, a);
        f[i] = a;
    }
    print(fps_inv(f));
}

signed main() {
    solve();
    return 0;
}
