#ifndef AJAY_COUNT_LABELED_STRONG_DIGRAPH
#define AJAY_COUNT_LABELED_STRONG_DIGRAPH

#include "library/poly/fps_inv.hpp"
#include "library/poly/fps_log.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_strong_digraph.hpp

// https://oeis.org/A003030
template <typename mint>
vector<mint> count_labeled_strong_digraph(int N) {
  vector<mint> F(N + 1, 1);
  // multiply by 2^n(n-1), then divide by 2^nC2
  mint pow = 1;
  For(i, 1, N + 1) F[i] = F[i - 1] * pow, pow += pow;
  For(i, N + 1) F[i] *= fact_inv<mint>(i);
  F = fps_inv<mint>(F);
  pow = 1;
  mint c = 1;
  For(i, N + 1) F[i] *= c, c *= pow, pow += pow;
  F = fps_log(F);
  For(i, N + 1) F[i] = -F[i] * fact<mint>(i);
  return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_STRONG_DIGRAPH
