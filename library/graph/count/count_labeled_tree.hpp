#ifndef AJAY_COUNT_LABELED_TREE
#define AJAY_COUNT_LABELED_TREE

#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_tree.hpp

// trees joining components of sizes (a1,a2,...,ak): prod(a)*N^{k-2}
// https://oeis.org/A000272
template <typename mint>
vector<mint> count_labeled_tree(long long nmax) {
  vector<mint> f(nmax + 1);
  For(n, 1, nmax + 1) { f[n] = (n == 1 ? mint(1) : mint(n).pow(n - 2)); }
  return f;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_TREE
