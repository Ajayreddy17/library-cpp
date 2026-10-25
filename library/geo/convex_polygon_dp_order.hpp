#ifndef AJAY_CONVEX_POLYGON_DP_ORDER
#define AJAY_CONVEX_POLYGON_DP_ORDER

#include "library/geo/angle_sort.hpp"

namespace mitsuha{
/*
Point set is distinct.
edge(i,j): ordering such that transitions i->j form edges in strictly increasing argument order.
Typically, fixing v and forming v->v gives a convex polygon.
Usually degenerate cases are handled separately; a flag tracks whether 3+ edges were used to ensure non-degeneracy.
*/
template <typename P>
vector<pair<int, int>> convex_polygon_dp_order(vector<P> A) {
  int N = len(A);
  vector<pair<int, int>> E;
  For(i, N) For(j, N) {
    if (i == j) continue;
    assert(A[i] != A[j]);
    E.emplace_back(i, j);
  }

  sort(All(E), [&](auto& X, auto& Y) -> bool {
    auto [a, b] = X;
    auto [c, d] = Y;
    int k = angle_comp_3(A[b] - A[a], A[d] - A[c]);
    if (k == -1) return true;
    if (k == 1) return false;
    P dir = A[b] - A[a];
    return dir.dot(A[b] - A[d]) > 0;
  });
  return E;
}

} // namespace mitsuha
#endif // AJAY_CONVEX_POLYGON_DP_ORDER
