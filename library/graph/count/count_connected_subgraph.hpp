#ifndef AJAY_COUNT_CONNECTED_SUBGRAPH
#define AJAY_COUNT_CONNECTED_SUBGRAPH

#include "library/graph/base.hpp"
#include "library/setfunc/sps_log.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_connected_subgraph.hpp

// O(N^2 2^N)
template <typename T, int LIM>
vector<T> count_connected_subgraph(Graph<int, 0> G) {
  int N = G.N;
  assert(N <= LIM);
  vector<T> pw(G.M + 1, 1);
  For(i, G.M) pw[i + 1] = pw[i] + pw[i];

  // edge
  vector<int> E(1 << N);
  for (auto& e: G.edges) { E[(1 << e.frm) | (1 << e.to)]++; }
  For(i, N) For(s, 1 << N) {
    int t = s | 1 << i;
    if (s < t) E[t] += E[s];
  }

  // any graph
  vector<T> dp(1 << N);
  For(s, 1 << N) dp[s] = pw[E[s]];

  // connected
  return sps_log<T, LIM>(dp);
}
} // namespace mitsuha
#endif // AJAY_COUNT_CONNECTED_SUBGRAPH
