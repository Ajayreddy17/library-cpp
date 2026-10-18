#define PROBLEM "https://judge.yosupo.jp/problem/bitwise_xor_convolution"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/setfunc/xor_convolution.hpp>

using mint = modint998;

void solve() {
    LL(N);
    VT(mint, A, 1 << N);
    VT(mint, B, 1 << N);
    auto C = xor_convolution(A, B);
    print(C);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
