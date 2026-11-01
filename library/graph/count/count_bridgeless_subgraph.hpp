#ifndef AJAY_COUNT_BRIDGELESS_SUBGRAPH
#define AJAY_COUNT_BRIDGELESS_SUBGRAPH

#include "library/graph/count/count_connected_subgraph.hpp"
#include "library/setfunc/subset_convolution.hpp"
#include "library/setfunc/sps_exp.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_bridgeless_subgraph.hpp

// O(N^3 2^N). https://loj.ac/p/6730.
template <typename T, int LIM>
vector<T> count_bridgeless_subgraph(Graph<int, 0> G, bool connected = true) {
  int N = G.N;
  auto dp = count_connected_subgraph<T, LIM>(G);

  For(r, 1, N) {
    uint32_t nbd = 0;
    for (auto& e: G[r])
      if (e.to < r) nbd |= 1 << e.to;

    // before: no bridge in [0,r)
    // after: no bridge in [0,r]
    vector<T> f(1 << (N - 1));
    vector<T> g(1 << (N - 1));
    For(L, 1 << r) {
      For(R, 1 << (N - 1 - r)) {
        f[L | R << r] = dp[L | R << (1 + r)] * popcnt(L & nbd);
        g[L | R << r] = dp[L | R << (1 + r) | (1 << r)];
      }
    }

    For(s, 1 << (N - 1)) f[s] = -f[s];
    f = sps_exp<T, LIM - 1>(f);
    g = subset_convolution<T, LIM - 1>(f, g);
    For(L, 1 << r) {
      For(R, 1 << (N - 1 - r)) { dp[L | R << (1 + r) | (1 << r)] = g[L | R << r]; }
    }
  }
  if (!connected) dp = sps_exp<T, LIM>(dp);
  return dp;
}
} // namespace mitsuha
#endif // AJAY_COUNT_BRIDGELESS_SUBGRAPH
