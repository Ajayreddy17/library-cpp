#define PROBLEM "https://judge.yosupo.jp/problem/polynomial_taylor_shift"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/poly/poly_taylor_shift.hpp>

using mint = modint998;

void solve() {
    LL(N, c);
    VT(mint, A, N);
    print(poly_taylor_shift(A, mint(c)));
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
