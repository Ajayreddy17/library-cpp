#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"
#include <library/template.hpp>

#include <library/poly/convolution.hpp>
using mint = modint107;

void solve() {
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
