#define PROBLEM \
    "https://judge.yosupo.jp/problem/log_of_formal_power_series_sparse"
#include <library/template.hpp>

#include <library/poly/fps_log.hpp>
#include <library/mod/dynamic_modint.hpp>

void solve() {
    using mint = dmint;
    mint::set_mod(998244353);
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
