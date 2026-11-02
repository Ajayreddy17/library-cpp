#ifndef AJAY_TRANSITIVE_REDUCTION
#define AJAY_TRANSITIVE_REDUCTION

#include "library/graph/bitset/transitive_closure.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bitset/transitive_reduction.hpp

// input is a DAG; keep a minimum edge set with the same reachability
// N^3/w.
// https://codeforces.com/problemset/problem/1835/F
vector<Bit_Array> transitive_reduction(vector<Bit_Array> G) {
    using BS = Bit_Array;
    int N = len(G);
    vector<BS> H = transitive_closure(G);
    For(v, N) H[v][v] = 0;
    vector<BS> HH(N, BS(N));
    For(i, N) For(j, N) {
        if (H[i][j]) {
            HH[i] |= H[j];
        }
    }

    Frr(L, N) For(R, L + 1, N) {
        if (HH[L][R]) G[L][R] = 0;
    }
    return G;
}
} // namespace mitsuha
#endif // AJAY_TRANSITIVE_REDUCTION
