#ifndef AJAY_INCREMENTAL_CENTROID
#define AJAY_INCREMENTAL_CENTROID

#include "library/graph/datastructure/tree_abelgroup.hpp"
#include "library/datastructure/fastset.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/incremental_centroid.hpp

// fixed tree; vertex weights can be incremented by 1
// cent: centroid
// max_subtree
template <typename TREE>
struct Incremental_Centroid {
  TREE& tree;
  int N;
  int cent;
  pair<int, int> max_subtree;  // (adj, size)
  int wt_sm;
  Tree_AbelGroup<TREE, Monoid_Add<int>, 0, 0, 1> TA;
  FastSet ss;

  Incremental_Centroid(TREE& tree)
      : tree(tree),
        N(tree.N),
        cent(0),
        max_subtree(0, 0),
        wt_sm(0),
        TA(tree),
        ss(N) {}

  int get_subtree_wt(int v) {
    assert(v != cent);
    // direction of v as seen from cent
    if (tree.in_subtree(v, cent)) {
      return TA.prod_subtree(tree.jump(cent, v, 1));
    }
    return wt_sm - TA.prod_subtree(cent);
  }

  int move_to(int v) {
    // move from cent towards v on the compressed tree
    if (tree.in_subtree(v, cent)) {
      // lca of the weights in the direction of v
      int a = tree.jump(cent, v, 1);
      int L = tree.LID[a], R = tree.RID[a];
      L = ss.next(L), R = ss.prev(R - 1);
      int x = tree.V[L], y = tree.V[R];
      return tree.lca(x, y);
    }
    int L = tree.LID[cent], R = tree.RID[cent];
    int x = v;
    vector<int> I;
    I.emplace_back(ss.next(0));
    if (1 < L) I.emplace_back(ss.prev(L - 1));
    if (R < N - 1) I.emplace_back(ss.next(R));
    I.emplace_back(ss.prev(N - 1));
    for (auto&& idx : I) {
      if (idx == -1 || idx == N) continue;
      if (L <= idx && idx < R) continue;
      int y = tree.V[idx];
      x = tree.meet(x, y, cent);
    }
    return x;
  }

  void add(int v) {
    ss.insert(tree.LID[v]), TA.add(v, 1), wt_sm++;
    if (v == cent) return;
    int wt = get_subtree_wt(v);
    if (max_subtree.second < wt) max_subtree = {tree.jump(cent, v, 1), wt};
    if (2 * wt <= wt_sm) return;
    int k = wt;
    assert(wt_sm == 2 * k - 1);
    int to = move_to(v);
    max_subtree = {tree.jump(to, cent, 1), k - 1};
    cent = to;
  }
};
} // namespace mitsuha
#endif // AJAY_INCREMENTAL_CENTROID
