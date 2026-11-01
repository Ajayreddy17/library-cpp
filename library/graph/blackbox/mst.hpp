#ifndef AJAY_MST
#define AJAY_MST

#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/blackbox/mst.hpp

// Brouvka
// find_unused(v): among unused vertices, the one joined to v with minimum cost, as (w, cost)
// pair<int,COST>; return {-1,*} if none
// returns a maximal spanning forest
template <typename COST, typename F0, typename F1, typename F2>
vector<tuple<int, int, COST>> blackbox_mst(int N, F0 set_used, F1 set_unused, F2 find_unused) {
  using edge = tuple<int, int, COST>;
  UnionFind uf(N);
  vector<edge> res;
  while (1) {
    bool upd = 0;
    vector<vector<int>> comp(N);
    vector<edge> cand(N, {-1, -1, -1});
    For(v, N) comp[uf[v]].emplace_back(v);
    For(v, N) if (uf[v] == v) {
      for (auto&& x: comp[v]) { set_used(x); }
      for (auto&& x: comp[v]) {
        auto [y, cost] = find_unused(x);
        if (y == -1) continue;
        auto [a, b, c] = cand[v];
        if (a == -1 || cost < c) { cand[v] = {x, y, cost}; }
      }
      for (auto&& x: comp[v]) { set_unused(x); }
    }
    For(v, N) if (uf[v] == v) {
      auto [a, b, c] = cand[v];
      if (a == -1) continue;
      upd = 1;
      if (uf.merge(a, b)) res.emplace_back(a, b, c);
    }
    if (!upd) break;
  }
  return res;
}

// Brouvka
// https://atcoder.jp/contests/pakencamp-2023-day1/tasks/pakencamp_2023_day1_p
// version using an incremental data structure
// init: initialise the search structure; called about 2logN times
// add: add a vertex to the search target
// find: (w,wt) or (-1,*)
template <typename COST, typename F0, typename F1, typename F2>
vector<tuple<int, int, COST>> blackbox_mst_incremental(int N, F0 init, F1 add, F2 find) {
  using edge = tuple<int, int, COST>;
  UnionFind uf(N);
  vector<edge> res;
  while (uf.n_comp > 1) {
    bool upd = 0;
    vector<vector<int>> comp(N);
    For(v, N) comp[uf[v]].emplace_back(v);
    vector<COST> wt(N, infty<COST>);
    vector<pair<int, int>> who(N, {-1, -1});
    init();
    For(i, N) {
      for (auto& v: comp[i]) {
        auto [w, x] = find(v);
        if (w != -1 && chmin(wt[i], x)) who[i] = {v, w};
      }
      for (auto& v: comp[i]) { add(v); }
    }
    init();
    Frr(i, N) {
      for (auto& v: comp[i]) {
        auto [w, x] = find(v);
        if (w != -1 && chmin(wt[i], x)) who[i] = {v, w};
      }
      for (auto& v: comp[i]) { add(v); }
    }
    For(i, N) {
      auto [a, b] = who[i];
      if (a == -1) continue;
      if (uf.merge(a, b)) {
        upd = true;
        res.emplace_back(a, b, wt[i]);
      }
    }
    if (!upd) break;
  }
  return res;
}

// Prim
// O(N^2)
// https://atcoder.jp/contests/pakencamp-2023-day1/tasks/pakencamp_2023_day1_p
template <typename COST, typename F>
vector<tuple<int, int, COST>> mst_prim(int N, F cost) {
  using edge = tuple<int, int, COST>;
  UnionFind uf(N);
  vector<edge> res;
  vector<long long> wt(N, infty<long long>);
  vector<int> TO(N);
  auto add = [&](int v) -> void {
    For(w, N) {
      if (TO[w] != -1 && chmin(wt[w], cost(v, w))) TO[w] = v;
    }
    wt[v] = infty<long long>;
    TO[v] = -1;
  };
  add(0);
  For(N - 1) {
    int v = min_element(All(wt)) - wt.begin();
    res.emplace_back(TO[v], v, wt[v]);
    add(v);
  }
  return res;
}
} // namespace mitsuha
#endif // AJAY_MST
