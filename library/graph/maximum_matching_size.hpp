#ifndef AJAY_MAXIMUM_MATCHING_SIZE
#define AJAY_MAXIMUM_MATCHING_SIZE

#include "library/random/base.hpp"
#include "library/mod/modint61.hpp"
#include "library/linear_algebra/matrix_rank.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_matching_size.hpp

template <typename GT>
int maximum_matching_size(GT& G) {
  static_assert(!GT::is_directed);
  using mint = modint61;
  int N = G.N;
  vector<vector<mint>> tutte(N, vector<mint>(N));
  for (auto&& e: G.edges) {
    mint x = RNG(mint::get_mod());
    int i = e.frm, j = e.to;
    tutte[i][j] += x;
    tutte[j][i] -= x;
  }
  return matrix_rank(tutte, N, N) / 2;
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_MATCHING_SIZE
