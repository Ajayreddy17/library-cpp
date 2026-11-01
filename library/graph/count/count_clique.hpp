#ifndef AJAY_COUNT_CLIQUE
#define AJAY_COUNT_CLIQUE

#include "library/graph/count/count_independent_set.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_clique.hpp

// 24 ms for (n,m) = (1000,1000)
// https://contest.ucup.ac/contest/1358/problem/7514
template <typename GT>
uint64_t count_clique(GT& G) {
  static_assert(!GT::is_directed);
  int N = G.N;
  uint64_t ANS = 1; // emptyset
  vector<int> new_idx(N, -1);

  while (N) {
    auto deg = G.deg_array();
    int p = min_element(All(deg)) - deg.begin();
    vector<int> nbd, other;
    for (auto& e: G[p]) nbd.emplace_back(e.to);
    For(v, N) {
      if (v != p) other.emplace_back(v);
    }

    // count independent sets in the complement of the neighbourhood graph
    int n = len(nbd);
    For(i, n) { new_idx[nbd[i]] = i; }
    vector<vector<int>> adj(n, vector<int>(n));
    for (auto& e: G.edges) {
      int a = e.frm, b = e.to;
      a = new_idx[a], b = new_idx[b];
      if (a == -1 || b == -1) continue;
      adj[a][b] = adj[b][a] = 1;
    }
    For(i, n) { new_idx[nbd[i]] = -1; }
    Graph<int, 0> G1(n);
    For(i, n) For(j, i) if (!adj[i][j]) G1.add(i, j);
    G1.build();
    uint64_t cnt = count_independent_set(G1);
    ANS += cnt;
    G = G.rearrange(other);
    assert(G.N == N - 1);
    --N;
  }
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_COUNT_CLIQUE
