#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"
#include <library/template.hpp>

#include <library/mod/dynamic_modint.hpp>
#include <library/poly/convolution.hpp>

void solve() {
    using mint = dmint;
    dmint::set_mod(1e9 + 7);
    LL(N, M);
    VT(mint, A, N);
    VT(mint, B, M);
    auto ANS = convolution(A, B);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
