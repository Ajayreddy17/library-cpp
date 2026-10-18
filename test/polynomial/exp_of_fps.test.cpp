#define PROBLEM "https://judge.yosupo.jp/problem/exp_of_formal_power_series"
#include <library/template.hpp>

#include <library/poly/fps_exp.hpp>

using mint = modint998;

void solve() {
    LL(N);
    VT(mint, A, N);
    print(fps_exp(A));
}

signed main() {
    solve();
    return 0;
}
