#ifndef AJAY_BLOCK_CUT
#define AJAY_BLOCK_CUT

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/block_cut.hpp

// works on disconnected graphs; the BCT is then a forest
// a block is an equivalence class of edges, so
// - a loop is a block of one edge
// - an isolated vertex is attached to no block
struct Block_Cut {
  int N, M, NB;
  Graph<int, 0> BCT;
  vector<int> comp_e;
  vector<bool> art;

  template <typename GT>
  Block_Cut(const GT& G) {
    N = G.N, M = G.M;
    vector<int> low(N), ord(N), st;
    vector<bool> used(N), used_e(M);
    st.reserve(M);
    art.assign(N, false);
    int k = 0;
    vector<vector<int>> es;
    For(r, N) if (!used[r]) {
      auto dfs = [&](auto& dfs, int v) -> void {
        used[v] = 1;
        low[v] = ord[v] = k++;
        int n_ch = 0;
        for (auto&& e : G[v]) {
          if (used_e[e.id]) continue;
          used_e[e.id] = 1;
          if (e.to == v) {
            es.emplace_back(vector<int>({e.id}));
            continue;
          }
          if (!used[e.to]) {
            ++n_ch;
            int s = len(st);
            st.emplace_back(e.id);
            dfs(dfs, e.to);
            chmin(low[v], low[e.to]);
            if (low[e.to] >= ord[v]) {
              if (v != r) art[v] = 1;
              vector<int> E;
              while (len(st) > s) E.emplace_back(POP(st));
              es.emplace_back(E);
            }
          } else {
            // back edge
            st.emplace_back(e.id);
            chmin(low[v], ord[e.to]);
          }
        }
        if (v == r) art[v] = (n_ch >= 2);
      };
      dfs(dfs, r);
      assert(st.empty());
    }

    comp_e.resize(M);
    For(v, N) used[v] = 0;
    NB = len(es);
    BCT.build(N + NB);
    vector<int> V;
    For(k, NB) {
      V.clear();
      for (int e : es[k]) {
        comp_e[e] = N + k;
        V.emplace_back(G.edges[e].frm);
        V.emplace_back(G.edges[e].to);
      }
      for (int v : V) {
        if (used[v]) continue;
        used[v] = 1;
        BCT.add(N + k, v);
      }
      for (int v : V) used[v] = 0;
    }
    BCT.build();
  }

  int edge_to_block(int eid) const {
    assert(0 <= eid && eid < M);
    return comp_e[eid];
  }

  bool is_art(int v) const {
    assert(0 <= v && v < N);
    return art[v];
  }
};
} // namespace mitsuha
#endif // AJAY_BLOCK_CUT
