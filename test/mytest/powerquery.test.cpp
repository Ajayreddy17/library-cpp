#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/datastructure/power_query.hpp"
#include "library/mod/modint.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  mint base = RNG(0, mint::get_mod());
  Power_Query<Monoid_Mul<mint>> X(base);
  For(1000) {
    long long exp = RNG(0, 1'000'000'000'000'000'000);
    mint a = base.pow(exp);
    mint b = X(exp);
    assert(a == b);
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  For(10) test();
  solve();
  return 0;
}
