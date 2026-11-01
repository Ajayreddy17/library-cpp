#ifndef AJAY_COMPLEMENT_GRAPH_DISTANCE
#define AJAY_COMPLEMENT_GRAPH_DISTANCE

#include "library/graph/blackbox/complement_graph_bfs.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/blackbox/complement_graph_distance.hpp

// list pairs at distance >= 2 as {v, w, dvw}
template <typename GT>
vector<tuple<int, int, int>> complement_graph_all_distances(GT& G) {
  const int N = G.N;
  auto deg = G.deg_array();
  int S = min_element(deg.begin(), deg.end()) - deg.begin();
  vector<int> nbd(N);
  for (auto& e : G[S]) {
    nbd[e.to] = 1;
  }

  vector<tuple<int, int, int>> res;
  for (auto& e : G.edges) {
    if (nbd[e.frm] || nbd[e.to]) continue;
    // a -> S -> b
    int a = e.frm, b = e.to;
    res.emplace_back(a, b, 2);
  }

  for (auto& e : G[S]) {
    int a = e.to;
    auto dist = complement_graph_bfs(G, a).first;
    For(b, N) {
      if (dist[b] <= 1) continue;
      if (nbd[b] && a >= b) continue;
      res.emplace_back(a, b, dist[b]);
    }
  }
  return res;
}
} // namespace mitsuha
#endif // AJAY_COMPLEMENT_GRAPH_DISTANCE
