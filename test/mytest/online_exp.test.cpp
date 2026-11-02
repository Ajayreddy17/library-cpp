#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/random/base.hpp"
#include "library/poly/fps_exp.hpp"
#include "library/poly/online/online_exp.hpp"

using mint = modint998;

void test() {
    auto gen = [&](int n) -> vector<mint> {
        vector<mint> f(n + 1);
        For(i, n + 1) f[i] = RNG(mint::get_mod());
        return f;
    };
    For(n, 1000) {
        vector<mint> f = gen(n);
        f[0] = mint(0);
        vector<mint> g = fps_exp<mint>(f);
        Online_Exp<mint> X;
        For(i, n + 1) { assert(g[i] == X.query(i, f[i])); }
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
