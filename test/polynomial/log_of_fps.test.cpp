#define PROBLEM "https://judge.yosupo.jp/problem/log_of_formal_power_series"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/poly/fps_log.hpp>

using mint = modint998;

void solve() {
    LL(N);
    VT(mint, f, N);
    auto ANS = fps_log(f);
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
