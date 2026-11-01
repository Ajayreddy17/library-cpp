#define PROBLEM  "https://judge.yosupo.jp/problem/composition_of_formal_power_series_large"
#include "library/template.hpp"
#include "library/poly/composition.hpp"

using mint = modint998;

void solve() {
  LL(N);
  vector<mint> Q(N); read(Q);
  vector<mint> P(N); read(P);
  auto F = composition(Q, P);
  print(F);
}

signed main() {
  solve();

  return 0;
}
