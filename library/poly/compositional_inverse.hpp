#ifndef AJAY_COMPOSITIONAL_INVERSE
#define AJAY_COMPOSITIONAL_INVERSE

#include "library/poly/diferentiate.hpp"
#include "library/poly/fps_div.hpp"
#include "library/poly/fps_pow.hpp"
#include "library/poly/power_projection.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/compositional_inverse.hpp
// #include "poly/composition.hpp"

// O(N^2)
// template <typename mint>
// vector<mint> compositional_inverse_old(const vector<mint>& F) {
//   const int N = len(F);
//   if (N == 0) return {};
//   assert(F[0] == mint(0));
//   if (N == 1) return F;
//   assert(F[0] == mint(0) && F[1] != mint(0));
//   vector<mint> DF = differentiate(F);

//   vector<mint> G(2);
//   G[1] = mint(1) / F[1];
//   while (len(G) < N) {
//     // G:= G(x)-(F(G(x))-x)/DF(G(x))
//     int n = len(G);
//     vector<mint> G1, G2;
//     {
//       vector<mint> FF(2 * n), GG(2 * n), DFF(n);
//       For(i, min<int>(len(F), 2 * n)) FF[i] = F[i];
//       For(i, min<int>(len(DF), n)) DFF[i] = DF[i];
//       For(i, n) GG[i] = G[i];
//       G1 = composition(FF, GG);
//       G2 = composition(DFF, G);
//     }
//     G1 = {G1.begin() + n, G1.end()};
//     G1 = fps_div(G1, G2);
//     G.resize(2 * n);
//     For(i, n) G[n + i] -= G1[i];
//   }
//   G.resize(N);
//   return G;
// }

template <typename mint>
vector<mint> compositional_inverse(vector<mint> f) {
  const int n = len(f) - 1;
  if (n == -1) return {};
  assert(f[0] == mint(0));
  if (n == 0) return f;
  assert(f[1] != mint(0));
  mint c = f[1];
  mint ic = c.inverse();
  for (auto& x : f) x *= ic;
  vector<mint> wt(n + 1);
  wt[n] = 1;

  vector<mint> A = power_projection<mint>(wt, f, n);
  vector<mint> g(n);
  For(i, 1, n + 1) g[n - i] = mint(n) * A[i] * inv<mint>(i);
  g = fps_pow_1<mint>(g, -inv<mint>(n));
  g.insert(g.begin(), 0);

  mint pow = 1;
  For(i, len(g)) g[i] *= pow, pow *= ic;
  return g;
}

// pass G->F(G) and G->DF(G)
// computes up to len(G); computing up to len(F) is also fine
// complexity is about the same as composition
template <typename mint, typename F1, typename F2>
vector<mint> compositional_inverse(const vector<mint>& F, F1 comp_F, F2 comp_DF) {
  const int N = len(F);
  assert(N <= 0 || F[0] == mint(0));
  assert(N <= 1 || F[1] != mint(0));

  vector<mint> G(2);
  G[1] = mint(1) / F[1];
  while (len(G) < N) {
    int n = len(G);
    // G:= G(x)-(F(G(x))-x)/DF(G(x))
    vector<mint> G2 = comp_DF(G);
    G.resize(2 * n);
    vector<mint> G1 = comp_F(G);
    G1 = {G1.begin() + n, G1.end()};
    G1 = fps_div(G1, G2);
    For(i, n) G[n + i] -= G1[i];
  }
  G.resize(N);
  return G;
}
} // namespace mitsuha
#endif // AJAY_COMPOSITIONAL_INVERSE
