#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
#include <library/template.hpp>

#include <library/poly/fps_exp.hpp>
#include <library/mod/dynamic_modint.hpp>

using mint = dmint;

void solve() {
    mint::set_mod(998244353);
    LL(N);
    VT(mint, A, N);
    print(fps_exp(A));
}

signed main() {
    solve();
    return 0;
}
