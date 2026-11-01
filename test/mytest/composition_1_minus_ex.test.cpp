#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/poly/composition_f_1_minus_ex.hpp"
#include "library/poly/composition.hpp"
#include "library/mod/modint.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  auto gen = [&](int n) -> vector<mint> {
    vector<mint> f(n + 1);
    For(i, n + 1) f[i] = RNG(mint::get_mod());
    return f;
  };
  For(n, 100) {
    vector<mint> f = gen(n);
    vector<mint> g(n + 1);
    For(i, 1, n + 1) g[i] = -fact_inv<mint>(i);
    vector<mint> F = composition_1_minus_ex(f);
    vector<mint> G = composition(f, g);
    assert(F == G);
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
