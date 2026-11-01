#ifndef AJAY_SERIES_PARALLEL
#define AJAY_SERIES_PARALLEL

#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/series_parallel.hpp
// https://codeforces.com/problemset/problem/2206/G
// two-terminal series parallel
// series: contract paths through degree-2 vertices
// parallel: contract multi-edges
// the process of reducing to a single edge st is represented as a tree
struct Series_Parallel {
  enum Etype { EDGE, S, P };
  struct Node {
    int s, t;
    Etype type;
    vector<int> ch;
  };

  int N, M, root;
  vector<Node> nodes;

  template <typename F>
  Series_Parallel(int N, int M, F f) : N(N), M(M), root(-1) {
    VtoE.resize(N), deg.resize(N);
    For(i, M) {
      auto [a, b] = f(i);
      add_node(a, b, Etype::EDGE, {});
    }
  }

  // returns false if not a series-parallel graph
  bool build(int S, int T) {
    if (deg[S] == 0 || deg[T] == 0) return false;
    while (1) {
      if (len(queS)) {
        int v = POP(queS);
        if (v == S || v == T || deg[v] != 2) continue;
        int a = -1, b = -1;
        for (auto& idx : VtoE[v]) {
          if (!active[idx]) continue;
          (a == -1 ? a : b) = idx;
        }
        int s = nodes[a].s + nodes[a].t - v;
        int t = nodes[b].s + nodes[b].t - v;
        if (s == t) return false;
        deactivate(a), deactivate(b);
        add_node(s, t, Etype::S, {a, b});
        continue;
      }
      if (len(queP)) {
        pair<int, int> k = POP(queP);
        vector<int> E = PtoE[k];
        if (len(E) <= 1) continue;
        assert(len(E) >= 2);
        PtoE[k].clear();
        for (auto& idx : E) deactivate(idx);
        add_node(k.first, k.second, Etype::P, E);
        continue;
      }
      break;
    }
    // success?
    For(i, len(nodes) - 1) if (active[i]) return false;

    // normalize
    vector<bool> vis(len(nodes));
    auto dfs = [&](auto& dfs, int i, int s) -> void {
      vis[i] = 1;
      // normalise nodes[i]
      if (nodes[i].s != s) {
        swap(nodes[i].s, nodes[i].t);
        reverse(nodes[i].ch.begin(), nodes[i].ch.end());
      }
      assert(nodes[i].s == s);
      if (nodes[i].type == Etype::EDGE) return;
      // re-list only children of a different type
      vector<int> ch;
      auto F = [&](auto& F, int j, int s) -> void {
        if (nodes[j].s != s) {
          swap(nodes[j].s, nodes[j].t);
          reverse(nodes[j].ch.begin(), nodes[j].ch.end());
        }
        if (nodes[i].type != nodes[j].type) {
          ch.emplace_back(j);
        } else {
          int v = s;
          for (int k : nodes[j].ch) {
            F(F, k, v);
            v = (nodes[j].type == Etype::P ? v : nodes[k].t);
          }
        }
      };
      F(F, i, s);
      nodes[i].ch = ch;
      int v = s;
      for (auto& c : ch) {
        dfs(dfs, c, v);
        v = (nodes[i].type == Etype::P ? v : nodes[c].t);
      }
    };
    dfs(dfs, len(nodes) - 1, S);

    // drop ignored nodes
    vector<int> new_idx(len(nodes), -1);
    int p = 0;
    For(i, len(nodes)) if (vis[i]) { new_idx[i] = p, nodes[p] = nodes[i], p++; }
    nodes.resize(p);
    For(i, p) {
      for (auto& j : nodes[i].ch) j = new_idx[j];
    }
    root = p - 1;
    return true;
  }

 private:
  vector<int> deg;
  vector<bool> active;
  map<pair<int, int>, vector<int>> PtoE;
  vector<vector<int>> VtoE;
  vector<pair<int, int>> queP;
  vector<int> queS;

  void add_node(int s, int t, Etype type, vector<int> ch) {
    if (s > t) {
      swap(s, t);
      reverse(ch.begin(), ch.end());
    }
    int idx = len(nodes);
    nodes.emplace_back(Node{s, t, type, ch});
    active.emplace_back(true);

    // pair -> edges
    pair<int, int> k = {s, t};
    PtoE[k].emplace_back(idx);
    VtoE[s].emplace_back(idx), VtoE[t].emplace_back(idx);
    deg[s]++, deg[t]++;

    if (len(PtoE[k]) >= 2) queP.emplace_back(k);
    if (deg[s] == 2) queS.emplace_back(s);
    if (deg[t] == 2) queS.emplace_back(t);
  }

  void deactivate(int k) {
    int s = nodes[k].s, t = nodes[k].t;
    deg[s]--, deg[t]--, active[k] = false;
  }
};
} // namespace mitsuha
#endif // AJAY_SERIES_PARALLEL
