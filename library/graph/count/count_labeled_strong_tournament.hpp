#ifndef AJAY_COUNT_LABELED_STRONG_TOURNAMENT
#define AJAY_COUNT_LABELED_STRONG_TOURNAMENT

#include "library/poly/fps_inv.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_strong_tournament.hpp

// https://oeis.org/A054946
template <typename mint>
vector<mint> count_labeled_strong_tournament(int N) {
  vector<mint> F(N + 1, 1);
  mint pow = 1;
  For(n, 1, N + 1) F[n] = F[n - 1] * pow, pow += pow;
  For(n, N + 1) F[n] *= fact_inv<mint>(n);
  F = fps_inv(F);
  F[0] = 0;
  For(n, N + 1) F[n] = -F[n] * fact<mint>(n);
  return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_STRONG_TOURNAMENT
