#ifndef AJAY_COUNT_LABELED_BICONNECTED
#define AJAY_COUNT_LABELED_BICONNECTED

#include "library/graph/count/count_labeled_connected.hpp"
#include "library/poly/compositional_inverse.hpp"
#include "library/poly/composition.hpp"
#include "library/poly/integrate.hpp"
#include "library/poly/fps_pow.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_biconnected.hpp

// https://oeis.org/A013922
template <typename mint>
vector<mint> count_labeled_biconnected(int N) {
  vector<mint> C = count_labeled_connected<mint>(N);
  For(i, N + 1) C[i] *= fact_inv<mint>(i);

  vector<mint> D(N);
  For(i, N) D[i] = C[i + 1] * mint(i + 1);

  vector<mint> E(N);
  For(i, N) E[i] = C[i] * mint(i);

  vector<mint> G = fps_log(D);

  vector<mint> IE = compositional_inverse(E);
  vector<mint> B = composition(G, IE);
  vector<mint> A = integrate(B);

  For(i, N + 1) A[i] *= fact<mint>(i);
  return A;
}

// https://oeis.org/A013922
template <typename mint>
mint count_labeled_biconnected_single(int N) {
  if (N < 2) return 0;
  vector<mint> C = count_labeled_connected<mint>(N);
  For(i, N + 1) C[i] *= fact_inv<mint>(i);

  vector<mint> D(N);
  For(i, N) D[i] = C[i + 1] * mint(i + 1);

  vector<mint> E(N);
  For(i, N) E[i] = C[i] * mint(i);

  vector<mint> G = fps_log(D);

  // (N-1)[x^{N-1}]G(IE(x))=[x^{-1}]G'(x)E(x)^{-(N-1)}
  // =[x^{N-2}]G'(x)(E(x)/x)^{-(N-1)}
  G = differentiate(G);
  E.erase(E.begin());
  E = fps_pow_1<mint>(E, -(N - 1));
  mint ANS = 0;
  For(i, N - 1) ANS += G[i] * E[N - 2 - i];

  ANS *= inv<mint>(N - 1);
  ANS *= inv<mint>(N);
  ANS *= fact<mint>(N);
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_BICONNECTED
