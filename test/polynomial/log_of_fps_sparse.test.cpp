#define PROBLEM \
    "https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse"
#include <library/template.hpp>

#include <library/poly/fps_log.hpp>

using mint = modint998;

void solve() {
    LL(N, K);
    vector<mint> f(N);
    For(K) {
        LL(i, a);
        f[i] = a;
    }
    print(fps_log(f));
}

signed main() {
    solve();

    return 0;
}
