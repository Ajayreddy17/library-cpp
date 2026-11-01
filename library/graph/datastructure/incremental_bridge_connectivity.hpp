#ifndef AJAY_INCREMENTAL_BRIDGE_CONNECTIVITY
#define AJAY_INCREMENTAL_BRIDGE_CONNECTIVITY

#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/ds/incremental_bridge_connectivity.hpp

// O((N + Q) log N)
struct Incremental_Bridge_Connectivity {
  int n;
  UnionFind uf, uf_bridge;

  Incremental_Bridge_Connectivity(int n)
      : n(n), uf(n), uf_bridge(n), par(n, -1), vis(n, -1), time(0) {}

  void add_edge(int a, int b) {
    a = uf_bridge[a], b = uf_bridge[b];
    if (a == b) return;
    if (uf[a] != uf[b]) {
      // bridge
      if (uf.size(a) < uf.size(b)) swap(a, b);
      make_root(b);
      par[b] = a;
      uf.merge(a, b);
      return;
    }

    // cycle. some 2ecc are merged.
    P.clear(), Q.clear();
    int t = time++;
    int p = a, q = b;
    int lca = [&]() -> int {
      while (1) {
        if (p != -1) {
          if (vis[p] == t) return p;
          P.emplace_back(p), vis[p] = t, p = par[p];
          if (p != -1) p = uf_bridge[p];
        }
        if (q != -1) {
          if (vis[q] == t) return q;
          Q.emplace_back(q), vis[q] = t, q = par[q];
          if (q != -1) q = uf_bridge[q];
        }
      }
      assert(0);
      return -1;
    }();
    // merge path
    for (int i : P) {
      uf_bridge.merge(i, a);
      if (i == lca) break;
    }
    for (int i : Q) {
      uf_bridge.merge(i, a);
      if (i == lca) break;
    }
    int r = uf_bridge[a];
    par[r] = par[lca];
  }

  int count_bridges() const { return uf_bridge.n_comp - uf.n_comp; }

 private:
  // forest of 2-edge-connected components
  // keeps the parent of each uf_bridge representative correct
  // the target is one vertex of the forest, so passing it through uf_bridge makes it correct
  vector<int> par;
  void make_root(int v) {
    assert(uf_bridge[v] == v);
    int ch = -1;
    while (v != -1) {
      int p = par[v];
      if (p != -1) p = uf_bridge[p];
      par[v] = ch;
      ch = v;
      v = p;
    }
  }

  // for lca
  vector<int> vis;
  vector<int> P, Q;
  int time;
};
} // namespace mitsuha
#endif // AJAY_INCREMENTAL_BRIDGE_CONNECTIVITY
