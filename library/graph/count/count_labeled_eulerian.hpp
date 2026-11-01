#ifndef AJAY_COUNT_LABELED_EULERIAN
#define AJAY_COUNT_LABELED_EULERIAN

#include "library/poly/fps_log.hpp"
#include "library/graph/count/count_labeled_undirected.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_eulerian.hpp

// https://oeis.org/A033678
template <typename mint>
vector<mint> count_labeled_eulerian(int N) {
  if (N == 0) return vector<mint>({mint(1)});
  vector<mint> F = count_labeled_undirected<mint>(N - 1);
  F.insert(F.begin(), mint(1));
  For(n, N + 1) F[n] *= fact_inv<mint>(n);
  F = fps_log(F);
  For(n, N + 1) F[n] *= fact<mint>(n);
  return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_EULERIAN
