#ifndef AJAY_FIND_EVEN_CYCLE
#define AJAY_FIND_EVEN_CYCLE

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/find_even_cycle.hpp

// (vs, es), size=(n+1,n)
// [R] https://scoreboard.icpc.global/46/finals46.pdf
template <typename GT>
pair<vector<int>, vector<int>> find_even_cycle(GT &G) {
  /*
  reference: https://chaoxu.prof/posts/2014-03-08-even-cycle-in-a-simple-graph.html
  build a DFS tree; each back edge gives a cycle
  an even cycle among them -> done
  two odd cycles sharing an edge -> their XOR works
  neither -> a cactus of odd cycles -> impossible
  */
  static_assert(!GT::is_directed);
  const int N = G.N, M = G.M;
  if (N == 1) return {{}, {}};
  bool FOUND = 0;
  vector<vector<int>> cyc; // edge index
  vector<int> in_cycle(M, -1);
  vector<int> path_v;
  vector<int> path_e;
  vector<bool> used(M);
  vector<bool> vis(N);
  pair<int, int> cid = {-1, -1};
  auto dfs = [&](auto &dfs, int v) -> void {
    for (auto &e: G[v]) {
      if (FOUND) return;
      if (used[e.id]) continue;
      used[e.id] = 1;

      if (!vis[e.to]) {
        vis[e.to] = 1, used[e.id] = 1;
        path_v.emplace_back(e.to), path_e.emplace_back(e.id);
        dfs(dfs, e.to);
        if (FOUND) return;
        POP(path_v), POP(path_e);
        continue;
      }
      // found a cycle
      vector<int> EID = {e.id};
      int p = len(path_v) - 1;
      while (path_v[p] != e.to) { EID.emplace_back(path_e[--p]); }
      int k = len(cyc);
      cyc.emplace_back(EID);
      if (len(EID) % 2 == 0) {
        cid = {k, k}, FOUND = 1;
        return;
      }
      for (auto &i: EID) {
        if (in_cycle[i] != -1) {
          cid = {k, in_cycle[i]}, FOUND = 1;
          return;
        }
        in_cycle[i] = k;
      }
    }
  };
  For(v, N) {
    if (vis[v]) continue;
    path_v = {int(v)};
    vis[v] = 1;
    dfs(dfs, v);
  }
  if (!FOUND) return {{}, {}};
  vector<int> es = [&]() {
    if (cid.first == cid.second) { return cyc[cid.first]; }
    auto [a, b] = cid;
    vector<int> use(M);
    for (auto &e: cyc[a]) use[e] ^= 1;
    for (auto &e: cyc[b]) use[e] ^= 1;
    vector<int> es;
    For(i, M) if (use[i]) es.emplace_back(i);
    return es;
  }();

  vector<pair<int, int>> v_to_e(N, {-1, -1});
  auto add = [&](int v, int e) -> void {
    if (v_to_e[v].first == -1) {
      v_to_e[v].first = e;
    } else {
      v_to_e[v].second = e;
    }
  };
  for (auto &e: es) { add(G.edges[e].frm, e), add(G.edges[e].to, e); }

  int v0 = G.edges[es[0]].frm;
  vector<int> vs = {v0};
  es.clear(), es.shrink_to_fit();
  es = {v_to_e[v0].first};
  while (1) {
    int e = es.back();
    int a = G.edges[e].frm, b = G.edges[e].to;
    assert(vs.back() == a || vs.back() == b);
    if (vs.back() != a) swap(a, b);
    vs.emplace_back(b);
    if (b == v0) break;
    es.emplace_back(v_to_e[b].first + v_to_e[b].second - e);
  }
  return {vs, es};
}
} // namespace mitsuha
#endif // AJAY_FIND_EVEN_CYCLE
