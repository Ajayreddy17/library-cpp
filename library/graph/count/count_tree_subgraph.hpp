#ifndef AJAY_COUNT_TREE_SUBGRAPH
#define AJAY_COUNT_TREE_SUBGRAPH

#include "library/graph/base.hpp"
#include "library/setfunc/sps_exp.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_tree_subgraph.hpp

// O(N^22^N). https://atcoder.jp/contests/abc253/tasks/abc253_h
template <typename T, int LIM>
vector<T> count_tree_subgraph(Graph<int, 0> G) {
  int N = G.N;
  vector<T> dp(1 << N);
  For(r, N) {
    uint32_t nbd = 0;
    for (auto& e: G[r])
      if (e.to < r) nbd |= 1 << e.to;
    vector<T> f(1 << r);
    For(s, 1 << r) f[s] = dp[s] * popcount(nbd & s);
    f = sps_exp<T, LIM>(f);
    For(s, 1 << r) dp[s | 1 << r] = f[s];
  }
  return dp;
}
} // namespace mitsuha
#endif // AJAY_COUNT_TREE_SUBGRAPH
