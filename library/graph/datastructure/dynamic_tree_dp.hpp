#ifndef AJAY_DYNAMIC_TREE_DP
#define AJAY_DYNAMIC_TREE_DP

#include "library/graph/datastructure/static_toptree.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/dynamic_tree_dp.hpp

// simple version without reroot
// https://codeforces.com/contest/1172/problem/E
// https://codeforces.com/contest/1942/problem/H
// single(v) : cluster of v together with its parent edge
// rake(L,R) : keeps the boundary of L
// compress(L,R)  L,R in top-down order
template <typename TREE, typename TREE_DP>
struct Dynamic_Tree_DP {
  using X = typename TREE_DP::value_type;
  Static_TopTree<TREE> STT;
  vector<X> dp;

  template <typename F>
  Dynamic_Tree_DP(TREE& tree, F single) : STT(tree) {
    int N = tree.N;
    dp.resize(2 * N - 1);
    For(i, N) dp[i] = single(i);
    For(i, N, 2 * N - 1) update(i);
  }

  void set(int v, X x) {
    dp[v] = x;
    for (int i = STT.par[v]; i != -1; i = STT.par[i]) update(i);
  }

  X prod_all() { return dp.back(); }

 private:
  inline void update(int i) {
    X &L = dp[STT.lch[i]], &R = dp[STT.rch[i]];
    dp[i] =
        (STT.is_compress[i] ? TREE_DP::compress(L, R) : TREE_DP::rake(L, R));
  }
};
} // namespace mitsuha
#endif // AJAY_DYNAMIC_TREE_DP
