#define PROBLEM  "https://judge.yosupo.jp/problem/composition_of_formal_power_series_large"
#include "library/template.hpp"
#include "library/mod/dynamic_modint.hpp"
#include "library/poly/composition.hpp"
#include "library/util/maspy_compat.hpp"

using mint = dmint;

void solve() {
  mint::set_mod(998244353);
  LL(N);
  vector<mint> f(N); read(f);
  vector<mint> g(N); read(g);
  f = composition(f, g);
  print(f);
}

signed main() { solve(); }
