#ifndef AJAY_INTERVAL_GRAPH_UNIONFIND
#define AJAY_INTERVAL_GRAPH_UNIONFIND

#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/blackbox/interval_graph_unionfind.hpp


// intervals are [L, R); union those with nonempty intersection
// https://codeforces.com/contest/1209/problem/G1
UnionFind interval_graph_unionfind(int N, vector<int> L, vector<int> R) {
  // while some interval contains another, merge the inner one and delete it
  // once no containment remains, merge adjacent ones
  assert(len(L) == N && len(R) == N);
  vector<int> I(N);
  For(i, N) I[i] = i;

  sort(I.begin(), I.end(), [&](auto& a, auto& b) -> bool {
    if (L[a] != L[b]) return L[a] < L[b];
    return R[a] > R[b];
  });

  UnionFind uf(N);
  vector<int> keep;
  for (auto& j: I) {
    if (!keep.empty()) {
      int i = keep.back();
      if (R[j] <= R[i] && R[j] - L[j] < R[i] - L[i]) {
        uf.merge(i, j);
        continue;
      }
    }
    keep.emplace_back(j);
  }
  For(k, len(keep) - 1) {
    int i = keep[k], j = keep[k + 1];
    if (max(L[i], L[j]) < min(R[i], R[j])) uf.merge(i, j);
  }
  return uf;
}
} // namespace mitsuha
#endif // AJAY_INTERVAL_GRAPH_UNIONFIND
