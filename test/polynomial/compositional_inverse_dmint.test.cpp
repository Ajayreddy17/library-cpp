#define PROBLEM  "https://judge.yosupo.jp/problem/compositional_inverse_of_formal_power_series_large"
#include "library/template.hpp"
#include "library/mod/dynamic_modint.hpp"
#include "library/poly/power_projection.hpp"
#include "library/poly/compositional_inverse.hpp"
#include "library/util/maspy_compat.hpp"

using mint = dmint;

void solve() {
  mint::set_mod(998244353);
  LL(N);
  vector<mint> f(N); read(f);
  f = compositional_inverse(f);
  print(f);
}

signed main() { solve(); }
