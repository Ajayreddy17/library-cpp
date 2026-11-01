#ifndef AJAY_BFS_BITSET
#define AJAY_BFS_BITSET

#include "library/datastructure/bit_array.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bitset/bfs_bitset.hpp

// unweighted shortest paths on a dense graph
// pass the 0/1 adjacency matrix as vector<bitset>
// O(N^2/w)
// reference: (4000,4000), 4000 times within 2 s?
template <typename BITSET>
vector<int> bfs_bitset(vector<BITSET>& G, int s) {
  const int N = len(G);
  assert(0 <= s && s < N);
  vector<int> dist(N, infty<int>);
  BITSET unused, que;
  if constexpr (is_same_v<BITSET, Bit_Array>) {
    unused = BITSET(N, 1);
    que = BITSET(N, 0);
  }
  For(v, N) unused[v] = 1;
  que[s] = 1;

  int d = 0;
  while (1) {
    int p = que._Find_first();
    if (p >= N) break;
    BITSET nxt;
    if constexpr (is_same_v<BITSET, Bit_Array>) { nxt = BITSET(N); }
    while (p < N) {
      dist[p] = d;
      unused[p] = 0;
      nxt |= G[p];
      p = que._Find_next(p);
    }
    que = nxt & unused;
    ++d;
  }
  return dist;
}
} // namespace mitsuha
#endif // AJAY_BFS_BITSET
