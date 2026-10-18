#define PROBLEM \
    "https://judge.yosupo.jp/problem/pow_of_formal_power_series_sparse"
#include <library/template.hpp>

#include <library/poly/fps_pow.hpp>
#include <library/mod/dynamic_modint.hpp>

using mint = dmint;

void solve() {
    dmint::set_mod(998244353);
    LL(N, K, M);
    vector<mint> f(N);
    For(K) {
        LL(i, a);
        f[i] = a;
    }
    print(fps_pow(f, M));
}

signed main() {
    solve();

    return 0;
}
