#ifndef AJAY_TREE_OF_UNIONFIND
#define AJAY_TREE_OF_UNIONFIND

#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_of_unionfind.hpp

/*
build the tree of the merge process
component created by the q-th merge: N+q
add_root = true: merge everything at the end and add root N+Q
*/
Graph<int, 1> tree_of_unionfind(int N, vector<pair<int, int>> query,
                                bool add_root) {
  UnionFind uf(N);
  vector<int> root(N);
  iota(root.begin(), root.end(), 0);
  int Q = len(query);
  Graph<int, 1> G(N + Q + add_root);
  For(q, Q) {
    int v = N + q;
    auto [a, b] = query[q];
    a = uf[a], b = uf[b];
    G.add(v, root[a]);
    if (b != a) G.add(v, root[b]);
    uf.merge(a, b);
    root[uf[a]] = v;
  }
  if (add_root) {
    int r = N + Q;
    For(v, N) if (uf[v] == v) G.add(r, root[v]);
  }
  G.build();
  return G;
}
} // namespace mitsuha
#endif // AJAY_TREE_OF_UNIONFIND
