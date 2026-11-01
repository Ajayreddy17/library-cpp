#ifndef AJAY_SCC_BITSET
#define AJAY_SCC_BITSET

#include "library/graph/blackbox/scc.hpp"
#include "library/datastructure/bit_array.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bitset/scc_bitset.hpp

// SCC of a dense graph
// O(N^2/w)
// https://codeforces.com/contest/641/problem/F
pair<int, vector<int>> scc_bitset(vector<Bit_Array> G) {
  using BS = Bit_Array;
  const int N = len(G);
  vector<BS> RG(N, BS(N));
  For(i, N) For(j, N) RG[i][j] = 1 * G[j][i];

  BS b0(N, 1);
  BS b1(N, 1);

  auto set_used = [&](int v, bool rev) -> void {
    if (!rev) b0[v] = 0;
    if (rev) b1[v] = 0;
  };

  auto find_unused = [&](int v, bool rev) -> int {
    if (!rev) {
      BS &x = G[v];
      x &= b0.slice(0, len(x));
      int p = x.prev(len(x));
      x.resize(p + 1);
      return p;
    }
    BS &x = RG[v];
    x &= b1.slice(0, len(x));
    int p = x.prev(len(x));
    x.resize(p + 1);
    return p;
  };
  return blackbox_scc(N, set_used, find_unused);
}
} // namespace mitsuha
#endif // AJAY_SCC_BITSET
