#ifndef AJAY_TRANSITIVE_CLOSURE
#define AJAY_TRANSITIVE_CLOSURE

#include "library/datastructure/bit_array.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bitset/transitive_closure.hpp

// https://codeforces.com/contest/641/problem/F
// given a DAG, add an edge for every reachable pair
vector<Bit_Array> transitive_closure(vector<Bit_Array> G) {
  int N = len(G);
  For(i, N) G[i][i] = 1;
  For(k, N) {
    // G[i][k] and G[k][j]
    For(i, N) {
      if (!G[i][k]) continue;
      G[i] |= G[k];
    }
  }
  return G;
}
} // namespace mitsuha
#endif // AJAY_TRANSITIVE_CLOSURE
