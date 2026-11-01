#ifndef AJAY_STATIC_TOPTREE
#define AJAY_STATIC_TOPTREE

#include "library/graph/tree.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/static_toptree.hpp

/*
reference: joitour tatyam
simple version where every cluster has a virtual root
merge the N (vertex + edge) pieces into the whole tree plus the edge from the root to its parent.
single(v) : cluster of v together with its parent edge
rake(L,R) : keeps the boundary of L
compress(L,R)  x,y in top-down order
*/
template <typename TREE>
struct Static_TopTree {
  int N;
  TREE &tree;
  vector<int> par, lch, rch, A, B;  // A, B boundary (top-down)
  vector<bool> is_compress;

  Static_TopTree(TREE &tree) : tree(tree) { build(); }

  void build() {
    N = tree.N;
    par.assign(N, -1), lch.assign(N, -1), rch.assign(N, -1), A.assign(N, -1),
        B.assign(N, -1), is_compress.assign(N, 0);
    For(v, N) { A[v] = tree.parent[v], B[v] = v; }
    build_dfs(tree.V[0]);
    assert(len(par) == 2 * N - 1);
  }

  // aggregate value over the whole tree
  // single(v) : cluster of v together with its parent edge
  // rake(x, y) rake so that uv (top-down) is the boundary
  // compress(x,y)  (a,b] + (b,c] in top-down order
  template <typename TREE_DP, typename F>
  typename TREE_DP::value_type tree_dp(F single) {
    using Data = typename TREE_DP::value_type;
    auto dfs = [&](auto &dfs, int k) -> Data {
      if (0 <= k && k < N) return single(k);
      Data x = dfs(dfs, lch[k]), y = dfs(dfs, rch[k]);
      if (is_compress[k]) {
        assert(B[lch[k]] == A[rch[k]]);
        return TREE_DP::compress(x, y);
      }
      return TREE_DP::rake(x, y);
    };
    return dfs(dfs, 2 * N - 2);
  }

 private:
  int new_node(int l, int r, int a, int b, bool c) {
    int v = len(par);
    par.emplace_back(-1), lch.emplace_back(l), rch.emplace_back(r), A.emplace_back(a), B.emplace_back(b), is_compress.emplace_back(c);
    par[l] = par[r] = v;
    return v;
  }

  // height, node idx
  // compress reference: https://atcoder.jp/contests/abc351/editorial/9910
  // but the choice of heavy paths is not optimised
  pair<int, int> build_dfs(int v) {
    assert(tree.head[v] == v);
    auto path = tree.heavy_path_at(v);
    vector<pair<int, int>> stack;
    stack.emplace_back(0, path[0]);
    auto merge_last_two = [&]() -> void {
      auto [h2, k2] = POP(stack);
      auto [h1, k1] = POP(stack);
      stack.emplace_back(max(h1, h2) + 1, new_node(k1, k2, A[k1], B[k2], true));
    };

    For(i, 1, len(path)) {
      min_priority_queue<pair<int, int>> que;
      int k = path[i];
      que.emplace(0, k);
      for (auto &c : tree.collect_light(path[i - 1])) {
        que.emplace(build_dfs(c));
      }
      while (len(que) >= 2) {
        auto [h1, i1] = POP(que);
        auto [h2, i2] = POP(que);
        if (i2 == k) swap(i1, i2);
        int i3 = new_node(i1, i2, A[i1], B[i1], false);
        if (k == i1) k = i3;
        que.emplace(max(h1, h2) + 1, i3);
      }
      stack.emplace_back(POP(que));

      while (1) {
        int n = len(stack);
        if (n >= 3 && (stack[n - 3].first == stack[n - 2].first ||
                       stack[n - 3].first <= stack[n - 1].first)) {
          auto [h3, k3] = POP(stack);
          merge_last_two(), stack.emplace_back(h3, k3);
        }
        elif (n >= 2 && stack[n - 2].first <= stack[n - 1].first) {
          merge_last_two();
        }
        else break;
      }
    }
    while (len(stack) >= 2) {
      merge_last_two();
    }
    return POP(stack);
  }
};
} // namespace mitsuha
#endif // AJAY_STATIC_TOPTREE
