#ifndef AJAY_DIAL
#define AJAY_DIAL

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/shortest_path/dial.hpp
// not verified
template <typename Graph>
pair<vector<typename Graph::cost_type>, vector<int>> dial(Graph& G, int s) {
  using COST = typename Graph::cost_type;
  assert(G.is_prepared());
  long long W = 0;
  long long N = G.N;
  for (auto&& e: G.edges) chmax(W, e.cost);
  vector<int> S(N * W + 2, -1);
  vector<int> T(N * W + 2, -1);
  vector<int> prev(N);
  vector<int> nxt(N);
  vector<COST> dist(N, W * N + 1);
  vector<int> par(N, -1);
  dist[s] = 0;
  auto add = [&](long long v) -> void {
    long long d = dist[v];
    prev[v] = T[d];
    if (T[d] != -1) nxt[T[d]] = v;
    T[d] = v;
    if (S[d] == -1) S[d] = v;
    nxt[v] = -1;
  };
  auto rm = [&](long long v) -> void {
    long long d = dist[v];
    if (prev[v] != -1) nxt[prev[v]] = nxt[v];
    if (nxt[v] != -1) prev[nxt[v]] = prev[v];
    if (S[d] == v) S[d] = nxt[v];
    if (T[d] == v) T[d] = prev[v];
  };
  For(v, N) add(v);

  For(d, N * W) {
    long long v = S[d];
    while (v != -1) {
      for (auto&& e: G[v]) {
        long long w = e.to;
        long long dw = d + e.cost;
        if (dw < dist[w]) {
          par[w] = v;
          rm(w);
          dist[w] = dw;
          add(w);
        }
      }
      v = nxt[v];
    }
  }
  For(v, N) if (dist[v] > N * W) dist[v] = -1;
  return {dist, par};
}
} // namespace mitsuha
#endif // AJAY_DIAL
