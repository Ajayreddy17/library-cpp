#define PROBLEM "https://judge.yosupo.jp/problem/inv_of_formal_power_series"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/mod/dynamic_modint.hpp>

#include <library/poly/fps_inv.hpp>

void solve() {
    using mint = dmint;
    mint::set_mod(998244353);

    LL(N);
    VT(mint, F, N);
    print(fps_inv(F));
}

signed main() {
    solve();

    return 0;
}
