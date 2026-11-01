#ifndef AJAY_COUNT_SPANNING_TREE
#define AJAY_COUNT_SPANNING_TREE

#include "library/graph/base.hpp"
#include "library/linear_algebra/det.hpp"
#include "library/linear_algebra/blackbox/det.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_spanning_tree.hpp


template <typename mint, bool sparse, typename GT>
mint count_spanning_tree_undirected(GT& G) {
  int N = G.N;
  if constexpr (!sparse) {
    vector<vector<mint>> mat(N, vector<mint>(N));
    for (auto& e: G.edges) {
      int a = e.frm, b = e.to;
      mat[a][a] += 1, mat[a][b] -= 1, mat[b][a] -= 1, mat[b][b] += 1;
    }
    For(i, N) POP(mat[i]);
    POP(mat);
    return det(mat);
  } else {
    auto apply = [&](vector<mint>& f) -> vector<mint> {
      vector<mint> g(N);
      For(v, N - 1) {
        g[v] += f[v] * G.deg(v);
        for (auto& e: G[v]) g[e.to] -= f[v];
      }
      POP(g);
      return g;
    };
    return blackbox_det<mint>(N - 1, apply);
  }
}

// out-rooted-tree
template <typename mint, bool sparse, typename GT>
mint count_spanning_tree_directed(GT& G, int r) {
  int N = G.N;
  if constexpr (!sparse) {
    vector<vector<mint>> mat(N, vector<mint>(N));
    for (auto& e: G.edges) {
      int a = e.frm, b = e.to;
      mat[b][b] += 1, mat[a][b] -= 1;
    }
    For(i, N) mat[i].erase(mat[i].begin() + r);
    mat.erase(mat.begin() + r);
    return det(mat);
  } else {
    auto apply = [&](vector<mint>& f) -> vector<mint> {
      f.insert(f.begin() + r, 0);
      vector<mint> g(N);
      for (auto& e: G.edges) { g[e.to] += f[e.to], g[e.to] -= f[e.frm]; }
      g.erase(g.begin() + r);
      return g;
    };
    return blackbox_det<mint>(N - 1, apply);
  }
}
} // namespace mitsuha
#endif // AJAY_COUNT_SPANNING_TREE
