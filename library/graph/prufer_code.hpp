#ifndef AJAY_PRUFER_CODE
#define AJAY_PRUFER_CODE

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/prufer_code.hpp

// [0,n-1]^{n-1} with the last entry n-1 (n=1 ok)
vector<int> to_prufer_code(Graph<int, 0>& G) {
  // take the leaf with the smallest label -> append its parent
  int n = G.N;
  vector<int> par(n);
  {
    auto dfs = [&](auto& dfs, int v, int p) -> void {
      par[v] = p;
      for (auto& e: G[v])
        if (e.to != p) dfs(dfs, e.to, v);
    };
    dfs(dfs, n - 1, -1);
  }
  vector<int> deg = G.deg_array();
  vector<int> res(n - 1);
  int p = 0;
  int leaf = -1;
  For(i, n - 1) {
    if (leaf == -1) {
      while (deg[p] != 1) ++p;
      leaf = p;
    }
    res[i] = par[leaf];
    deg[leaf]--, deg[par[leaf]]--;
    leaf = (deg[par[leaf]] == 1 && par[leaf] < p ? par[leaf] : -1);
  }
  return res;
}

Graph<int, 0> from_prufer_code(vector<int> code) {
  int n = len(code) + 1;
  Graph<int, 0> G(n);
  if (n == 1) return G;
  assert(code.back() == n - 1);
  vector<int> deg(n, 1);
  for (auto& v: code) deg[v]++;

  int p = 0;
  int leaf = -1;
  For(i, n - 1) {
    if (leaf == -1) {
      while (deg[p] != 1) p++;
      leaf = p;
    }
    G.add(code[i], leaf);
    deg[leaf]--, deg[code[i]]--;
    leaf = (code[i] < p && deg[code[i]] == 1 ? code[i] : -1);
  }
  G.build();
  return G;
}
} // namespace mitsuha
#endif // AJAY_PRUFER_CODE
