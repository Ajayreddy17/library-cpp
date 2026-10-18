#define PROBLEM "https://judge.yosupo.jp/problem/division_of_polynomials"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/poly/poly_divmod.hpp>

using mint = modint998;

void solve() {
    LL(N, M);
    VT(mint, f, N);
    VT(mint, g, M);
    auto [q, r] = poly_divmod(f, g);
    print(len(q), len(r));
    print(q);
    print(r);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
