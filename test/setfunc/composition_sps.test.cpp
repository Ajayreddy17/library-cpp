#define PROBLEM  "https://judge.yosupo.jp/problem/polynomial_composite_set_power_series"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/setfunc/sps_composition.hpp"

using mint = modint998;
void solve() {
  LL(M, N);
  vector<mint> f(M); read(f);
  vector<mint> s(1 << N); read(s);
  s = sps_composition_poly<mint, 20>(f, s);
  print(s);
}

signed main() {
  solve();
  return 0;
}
