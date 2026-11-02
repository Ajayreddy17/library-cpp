#ifndef AJAY_BIPARTITE_BALANCED_EDGE_COLORING
#define AJAY_BIPARTITE_BALANCED_EDGE_COLORING

#include "library/graph/bipartite_edge_coloring.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/bipartite_balanced_edge_coloring.hpp

// colour edges with K colours so that around every vertex the counts are balanced (max-min <= 1)
// return : color[eid].
// https://codeforces.com/contest/212/problem/A
vector<int> bipartite_balanced_edge_coloring(Graph<int, 0> G, int K) {
    int N = G.N, M = G.M;
    vector<int> A, B;
    vector<int> cnt(N);
    vector<int> now(N);
    int nxt = 0;

    auto get = [&](int v) -> int {
        if (cnt[v] % K == 0) { now[v] = nxt++; }
        cnt[v]++;
        return now[v];
    };
    for (auto& e: G.edges) {
        int a = e.frm, b = e.to;
        a = get(a), b = get(b);
        A.emplace_back(a), B.emplace_back(b);
    }
    Graph<int, 0> H(nxt);
    For(i, M) H.add(A[i], B[i]);
    H.build();
    return bipartite_edge_coloring(H).second;
}
} // namespace mitsuha
#endif // AJAY_BIPARTITE_BALANCED_EDGE_COLORING
