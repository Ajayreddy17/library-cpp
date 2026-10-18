#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include <library/template.hpp>
#include <library/mod/dynamic_modint.hpp>
#include <library/poly/convolution.hpp>

using mint = dmint;

void solve() {
    mint::set_mod(998244353);
    mint::set_ntt_info();

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
