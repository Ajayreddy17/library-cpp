#define PROBLEM "https://judge.yosupo.jp/problem/product_of_polynomial_sequence"
#include <library/template.hpp>
#include <library/mod/modint.hpp>
#include <library/poly/convolution_all.hpp>

using mint = modint998;

void solve() {
    LL(N);
    vector<vector<mint>> polys;
    For(N) {
        LL(d);
        VT(mint, f, d + 1);
        polys.emplace_back(f);
    }
    auto f = convolution_all(polys);
    print(f);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
