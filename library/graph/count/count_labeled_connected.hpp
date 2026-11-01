#ifndef AJAY_COUNT_LABELED_CONNECTED
#define AJAY_COUNT_LABELED_CONNECTED

#include "library/graph/count/count_labeled_undirected.hpp"
#include "library/poly/fps_log.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_connected.hpp

// https://oeis.org/A001187
template <typename mint>
vector<mint> count_labeled_connected(int N) {
  vector<mint> F = count_labeled_undirected<mint>(N);
  For(i, N + 1) F[i] *= fact_inv<mint>(i);
  F = fps_log(F);
  For(i, N + 1) F[i] *= fact<mint>(i);
  return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_CONNECTED
