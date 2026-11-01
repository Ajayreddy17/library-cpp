#ifndef AJAY_COUNT_LABELED_BRIDGELESS
#define AJAY_COUNT_LABELED_BRIDGELESS

#include "library/graph/count/count_labeled_connected.hpp"
#include "library/poly/compositional_inverse.hpp"
#include "library/poly/composition.hpp"
#include "library/poly/fps_exp.hpp"
#include "library/poly/fps_pow.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_bridgeless.hpp

// connected graphs without bridges
// https://oeis.org/A095983
// N=1: 1
// O(Nlog^2N)
// https://atcoder.jp/contests/ttpc2023/tasks/ttpc2023_p
template <typename mint>
vector<mint> count_labeled_bridgeless(int N) {
  vector<mint> C = count_labeled_connected<mint>(N);
  For(i, N + 1) C[i] *= fact_inv<mint>(i);

  vector<mint> D(N + 1);
  For(i, N + 1) D[i] = mint(i) * C[i];

  vector<mint> E = fps_exp(D);
  E.insert(E.begin(), mint(0));
  E.pop_back();

  // D(x)=B(E(x))
  vector<mint> IE = compositional_inverse(E);
  vector<mint> B = composition(D, IE);

  vector<mint> A(N + 1);
  For(i, 1, N + 1) A[i] = B[i] * inv<mint>(i);

  For(i, 1, N + 1) A[i] *= fact<mint>(i);
  return A;
}

// https://oeis.org/A095983
// only the value at N, O(NlogN)
template <typename mint>
mint count_labeled_bridgeless_single(int N) {
  if (N == 0) return 0;
  vector<mint> C = count_labeled_connected<mint>(N);
  For(i, N + 1) C[i] *= fact_inv<mint>(i);

  vector<mint> D(N + 1);
  For(i, N + 1) D[i] = mint(i) * C[i];

  vector<mint> E = fps_exp(D);
  E.insert(E.begin(), mint(0));
  E.pop_back();

  // D(x)=B(E(x))
  // want [x^N]B(x)
  // Lagrange Inversion
  // N[x^N]D(IE(x))=[x^{-1}]D'(x)E(x)^{-N}
  // =[x^{N-1}]D'(x)(E(x)/x)^{-N}

  E.erase(E.begin());
  E = fps_pow_1<mint>(E, -N);
  D = differentiate(D);
  mint ANS = 0;
  For(i, N) ANS += D[i] * E[N - 1 - i];
  ANS *= inv<mint>(N);

  // got [x^N]B(x)
  ANS *= inv<mint>(N);
  ANS *= fact<mint>(N);
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_BRIDGELESS
