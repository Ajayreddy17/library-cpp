#define PROBLEM "https://yukicoder.me/problems/no/2587"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/base.hpp"
#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/coef_of_rational_fps.hpp"
#include "library/graph/tree_walk_generating_function.hpp"

using mint = modint998;
using poly = vector<mint>;

void solve() {
  LL(N, M, S, T);
  --S, --T;
  Graph<int, 0> G(N);
  G.read_tree();

  auto [g, f] = tree_walk_generating_function<false, mint>(
      G, S, T, [&](int i, int j) -> mint { return 1; });

  // W(x) = g(x)/f(x)
  // ANS = sum_m binom(M,m)[x^m]W(x)
  // EGF(W) = sum_m W_m 1/m! x^m
  // [x^M]e^x EGF(W)
  // 根が全部 -1 される
  // ANS も C-rec で、f(x-1)

  vector<mint> W = fps_div(g, f);
  For(m, N) W[m] *= fact_inv<mint>(m);
  vector<mint> tmp(N + 1);
  For(m, N + 1) tmp[m] = fact_inv<mint>(m);
  vector<mint> F = convolution(W, tmp);
  F.resize(N);
  For(i, N) F[i] *= fact<mint>(i);
  reverse(f.begin(), f.end());
  f = poly_taylor_shift<mint>(f, -1);
  reverse(f.begin(), f.end());
  g = convolution<mint>(F, f);
  g.resize(N);
  mint ANS = coef_of_rational_fps<mint>(g, f, M);
  print(ANS);
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
