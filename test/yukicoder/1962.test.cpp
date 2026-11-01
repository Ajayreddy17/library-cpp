#define PROBLEM "https://yukicoder.me/problems/no/1962"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/sum_of_rationals.hpp"
#include "library/poly/coef_of_rational_fps.hpp"

using mint = modint998;

void solve() {
  LL(N, M);
  using poly = vector<mint>;
  vector<pair<poly, poly>> dat;
  For(X, 1, M + 1) {
    poly num(X), den(X + 1);
    For(i, 1, X) num[i] = mint(1);
    For(i, X) den[i] = mint(1);
    den[X] = mint(-1);
    dat.emplace_back(num, den);
  }
  auto [f, g] = sum_of_rationals(dat);
  poly num = f;
  poly den = g;
  For(i, len(f)) den[i] -= f[i];
  mint ANS = coef_of_rational_fps<mint>(num, den, N);
  print(ANS);
}

signed main() {
  solve();
  return 0;
}
