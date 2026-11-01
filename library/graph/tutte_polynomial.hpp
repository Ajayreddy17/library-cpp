#ifndef AJAY_TUTTE_POLYNOMIAL
#define AJAY_TUTTE_POLYNOMIAL

#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/setfunc/sps_exp.hpp"
#include "library/setfunc/sps_composition.hpp"
#include "library/graph/base.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tutte_polynomial.hpp

template <typename mint, int NMAX>
mint tutte_polynomial_eval_connected(Graph<int, 0> G, mint X, mint Y) {
  int N = G.N;
  X -= 1, Y -= 1;
  /*
  Decompose V into connected components:
  for each subset S, the sum of y^{cycle} over edge sets A spanning S is
  F[S]; with c[n]F^n/n!, c[n] = X^{n-k(E)}, this is an EGF composition.

  computing F[S]:
  add vertices one at a time,
  weight each set by its number of edges and take exp,
  weights C(N,1) + C(N,2)Y + C(N,3)YY+...
  */

  vector<vector<mint>> bin(N + 1, vector<mint>(N + 1));
  bin[0][0] = 1;
  For(i, N)
  For(j, i + 1) bin[i + 1][j] += bin[i][j],
      bin[i + 1][j + 1] += bin[i][j];
  vector<mint> wt(N + 1);
  For(n, 1, N + 1) {
    mint pow = 1;
    For(m, 1, n + 1) { wt[n] += bin[n][m] * pow, pow *= Y; }
  }
  vector<mint> F(1 << N);
  For(v, N) {
    uint32_t nbd = 0;
    for (auto& e : G[v]) nbd |= 1 << e.to;
    vector<mint> f(1 << v);
    For(s, 1 << v) { f[s] = F[s] * wt[popcount(s & nbd)]; }
    f = sps_exp<mint, NMAX>(f);
    For(s, 1 << v) { F[s | 1 << v] = f[s]; }
  }
  if (X == mint(0)) {
    return F.back();
  }

  // works when X is not invertible; everything works over a ring
  // vector<mint> c(N + 1);
  // mint pow = 1;
  // For(n, 1, N + 1) { c[n] = pow, pow *= X; }
  // F = sps_composition_egf<mint, NMAX>(c, F);
  // return F.back();
  For(s, 1 << N) F[s] *= X;
  F = sps_exp<mint, NMAX>(F);
  return F.back() * X.inverse();
}

// QOJ 45
template <typename mint, int NMAX>
mint tutte_polynomial_eval(Graph<int, 0> G, mint X, mint Y) {
  int N = G.N;
  UnionFind uf(N);
  for (auto& e : G.edges) uf.merge(e.frm, e.to);
  vector<vector<int>> vs(N);
  For(v, N) vs[uf[v]].emplace_back(v);
  mint ANS = 1;
  for (auto& V : vs) {
    if (V.empty()) continue;
    Graph<int, 0> H = G.rearrange(V);
    ANS *= tutte_polynomial_eval_connected<mint, NMAX>(H, X, Y);
  }
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_TUTTE_POLYNOMIAL
