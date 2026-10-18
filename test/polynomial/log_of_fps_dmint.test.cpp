#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include <library/template.hpp>

#include <library/mod/dynamic_modint.hpp>
#include <library/poly/fps_log.hpp>

void solve() {
    using mint = dmint;
    mint::set_mod(998244353);
    LL(N);
    VT(mint, f, N);
    auto ANS = fps_log(f);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
