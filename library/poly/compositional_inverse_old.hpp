#ifndef AJAY_COMPOSITIONAL_INVERSE_OLD
#define AJAY_COMPOSITIONAL_INVERSE_OLD

#include "library/poly/diferentiate.hpp"
#include "library/poly/fps_div.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/compositional_inverse_old.hpp

// O(N ^ 2)
template <typename mint>
vector<mint> compositional_inverse_old(const vector<mint>& F) {
  const int N = len(F);
  if (N == 0) return {};
  assert(F[0] == mint(0));
  if (N == 1) return F;
  assert(F[0] == mint(0) && F[1] != mint(0));
  vector<mint> DF = differentiate(F);

  vector<mint> G(2);
  G[1] = mint(1) / F[1];
  while (len(G) < N) {
    // G:= G(x)-(F(G(x))-x)/DF(G(x))
    int n = len(G);
    vector<mint> G1, G2;
    {
      vector<mint> FF(2 * n), GG(2 * n), DFF(n);
      For(i, min<int>(len(F), 2 * n)) FF[i] = F[i];
      For(i, min<int>(len(DF), n)) DFF[i] = DF[i];
      For(i, n) GG[i] = G[i];
      G1 = composition(FF, GG);
      G2 = composition(DFF, G);
    }
    G1 = {G1.begin() + n, G1.end()};
    G1 = fps_div(G1, G2);
    G.resize(2 * n);
    For(i, n) G[n + i] -= G1[i];
  }
  G.resize(N);
  return G;
}
} // namespace mitsuha
#endif // AJAY_COMPOSITIONAL_INVERSE_OLD
