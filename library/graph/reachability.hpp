#ifndef AJAY_REACHABILITY
#define AJAY_REACHABILITY

#include "library/graph/strongly_connected_component.hpp"
#include "library/graph/reverse_graph.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/reachability.hpp

// reachability queries on a directed graph, O((N+M)Q/w)
template <typename GT, typename P>
vector<int> reachability(GT& G, vector<P> query) {
  using U = uint64_t;
  constexpr int W = 64;

  auto [C, comp] = strongly_connected_component(G);

  vector<pair<int, int>> edges;
  for (auto&& e : G.edges) {
    auto a = comp[e.frm], b = comp[e.to];
    assert(a <= b);
    if (a < b) edges.emplace_back(a, b);
  }
  settify(edges);
  for (auto& [a, b] : query) a = comp[a], b = comp[b];

  int Q = len(query);
  vector<int> ANS(Q);

  vector<int> S;
  vector<vector<int>> QID(C);
  For(q, Q) {
    auto [a, b] = query[q];
    if (a >= b) {
      ANS[q] = (a == b);
      continue;
    }
    QID[a].emplace_back(q);
    S.emplace_back(a);
  }

  settify(S);
  vector<U> dp(C);
  int p = 0;
  for (int l = 0; l < len(S); l += W) {
    int r = min<int>(l + W, len(S));
    fill(dp.begin() + S[l], dp.end(), U(0));
    For(i, r - l) { dp[S[l + i]] |= U(1) << i; }
    while (p < len(edges) && edges[p].first < S[l]) ++p;
    For(i, p, len(edges)) { dp[edges[i].second] |= dp[edges[i].first]; }
    For(i, r - l) {
      int s = S[l + i];
      for (auto& qid : QID[s]) {
        int t = query[qid].second;
        ANS[qid] = dp[t] >> i & 1;
      }
    }
  }
  return ANS;
}

// ANS[v] := count(reachable from v).
// (N,M)=(2e5,4e5): about 1.5 s; running time varies a lot
// https://codeforces.com/contest/2041/problem/K
vector<int> count_reachable(Graph<int, 1> G) {
  G = reverse_graph(G);
  int N = G.N;
  auto [nc, comp] = strongly_connected_component(G);
  vector<int> sz(nc);
  For(v, N) sz[comp[v]]++;

  // sorted pairs
  vector<pair<int, int>> E;
  for (auto& e : G.edges) {
    int a = comp[e.frm], b = comp[e.to];
    if (a != b) E.emplace_back(a, b);
  }
  settify(E);

  vector<int> ANS(nc);
  int p = 0;
  int idx = 0;
  vector<uint128> dp(nc);
  while (p < nc) {
    int k = 0;
    int s = p;
    memset(dp.data() + s, 0, (nc - s) * sizeof(uint128));
    while (k < 128 && p < nc) {
      if (sz[p] == 0) ++p;
      if (p == nc) break;
      int m = min(sz[p], 128 - k);
      for (int i = k; i < k + m; ++i) dp[p] |= uint128(1) << i;
      sz[p] -= m, k += m;
    }
    while (idx < len(E) && E[idx].first < s) ++idx;
    For(i, idx, len(E)) { dp[E[i].second] |= dp[E[i].first]; }
    For(v, s, nc) { ANS[v] += popcnt(uint64_t(dp[v] >> 64)) + popcnt(uint64_t(dp[v])); }
  }
  ANS = rearrange(ANS, comp);
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_REACHABILITY
