#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_formal_power_series"
#include <library/template.hpp>

#include <library/mod/dynamic_modint.hpp>
#include <library/poly/fps_pow.hpp>

using mint = modint998;

void solve() {
    using mint = dmint;
    mint::set_mod(998244353);
    LL(N, M);
    VT(mint, A, N);
    print(fps_pow(A, M));
}

signed main() {
    solve();
    return 0;
}
