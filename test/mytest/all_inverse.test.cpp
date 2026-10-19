#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>

#include <library/mod/modint.hpp>
#include <library/mod/all_inverse.hpp>

void test() {
    using mint = modint998;
    vector<int> ns = {1, 2, 3, 4, 5, 100000};

    for (auto&& N: ns) {
        vector<mint> X(N);
        For(i, N) { X[i] = RNG(1, mint::get_mod()); }
        auto Y = all_inverse(X);
        For(i, N) assert(X[i] * Y[i] == mint(1));
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();
    return 0;
}
