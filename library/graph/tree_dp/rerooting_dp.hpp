#ifndef AJAY_REROOTING_DP
#define AJAY_REROOTING_DP

#include "library/graph/tree.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_dp/rerooting_dp.hpp

template <typename TREE, typename Data>
struct Rerooting_DP {
  TREE& tree;

  // full tree rooted at v
  vector<Data> dp;
  // subtree v with respect to the original root
  vector<Data> dp_subtree;
  // component containing parent[v] after removing edge parent[v]-v,
  // rooted at parent[v]
  vector<Data> dp_parent;

  template <typename F1, typename F2, typename F3>
  Rerooting_DP(TREE& tree, F1 f_ee, F2 f_ev, F3 f_ve, const Data id)
      : tree(tree) {
    build(f_ee, f_ev, f_ve, id);
  }

  Data operator[](int v) const { return dp[v]; }

  // subtree v when the tree is rooted at root
  Data get(int v, int root) const {
    if (root == v) return dp[v];
    if (!tree.in_subtree(root, v)) return dp_subtree[v];
    int w = tree.jump(v, root, 1);
    return dp_parent[w];
  }

  template <typename F1, typename F2, typename F3>
  void build(F1 f_ee, F2 f_ev, F3 f_ve, const Data id) {
    int N = tree.N;
    dp.assign(N, id);
    dp_subtree.assign(N, id);
    dp_parent.assign(N, id);
    vector<int> ch;
    Frr(i, N) {
      int v = tree.V[i];
      Data X = id;
      tree.collect_child(v, ch);
      for (int c : ch) {
        dp[c] = X;
        dp_parent[c] = f_ve(dp_subtree[c], c, v);
        X = f_ee(X, dp_parent[c]);
      }
      dp_subtree[v] = f_ev(X, v);
    }
    For(i, N) {
      int v = tree.V[i];
      tree.collect_child(v, ch);

      Data X = id;
      if (tree.parent[v] != -1) {
        X = f_ve(dp_parent[v], tree.parent[v], v);
      }

      Frr(k, len(ch)) {
        int c = ch[k];
        Data Y = f_ee(dp[c], X);
        Data branch = dp_parent[c];
        X = f_ee(branch, X);
        dp_parent[c] = f_ev(Y, v);
      }
      dp[v] = f_ev(X, v);
    }
  }
};
} // namespace mitsuha
#endif // AJAY_REROOTING_DP
