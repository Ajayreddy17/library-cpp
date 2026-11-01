#define PROBLEM  "https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/compositional_inverse.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void solve() {
  LL(N);
  vector<mint> F(N); read(F);
  vector<mint> G = compositional_inverse(F);
  print(G);
}

signed main() {
  solve();
  return 0;
}
