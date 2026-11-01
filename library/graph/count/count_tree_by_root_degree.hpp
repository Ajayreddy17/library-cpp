#ifndef AJAY_COUNT_TREE_BY_ROOT_DEGREE
#define AJAY_COUNT_TREE_BY_ROOT_DEGREE

#include "library/mod/modint_common.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_tree_by_root_degree.hpp
/*
Count labelled trees by number of vertices and root degree.
*/
template <typename T>
T count_tree_by_root_degree(int n, int root_degree) {
  int d = root_degree;
  if (d <= 0 || d >= n) return T(0);
  d -= 1;
  return fact<T>(n - 2) * fact_inv<T>(d) * fact_inv<T>(n - 2 - d)
         * T(n - 1).pow(n - 2 - d);
}
} // namespace mitsuha
#endif // AJAY_COUNT_TREE_BY_ROOT_DEGREE
