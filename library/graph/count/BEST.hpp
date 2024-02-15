#ifndef AJAY_BEST_THEOREM
#define AJAY_BEST_THEOREM

#include "library/graph/base.hpp"
#include "library/linear_algebra/blackbox/det.hpp"

namespace mitsuha{
/*
Count the number of closed walks that pass through all points and edges starting from one selected edge.
Multiple edges are passed as vc<int>(eid), otherwise all 1. e.cost is not referenced.
Consider the edges with labels. If you want to equate multiple edges, divide by the factorial afterwards.
O(N^2+NM) (+ Multiply the multiplicity factorial at the end).
*/
template <typename mint, typename GT>
mint BEST_theorem(GT G, vector<int> edge_multiplicity = {}) {
    static_assert(GT::is_directed);
    int N = G.N, M = G.M;
    if (M == 0) return 0;
    if (edge_multiplicity.empty()) edge_multiplicity.assign(M, 1);
    vector<int> vs;
    for (auto& e: G.edges) {
        if (edge_multiplicity[e.id] == 0) continue;
        vs.emplace_back(e.frm), vs.emplace_back(e.to);
    }

    settify(vs);
    G = G.rearrange(vs, true);
    N = G.N;

    vector<int> indeg(N), outdeg(N);
    vector<tuple<int, int, mint>> mat;
    for (auto& e: G.edges) {
        int a = e.frm, b = e.to, x = edge_multiplicity[e.id];
        outdeg[a] += x, indeg[b] += x;
        if (a < N - 1 && b < N - 1) mat.emplace_back(a, b, -x);
        if (a < N - 1) mat.emplace_back(a, a, x);
    }
    For(v, N) if (indeg[v] != outdeg[v]) return 0;

    auto apply = [&](vector<mint> A) -> vector<mint> {
        vector<mint> B(N - 1);
        for (auto& [a, b, c]: mat) B[b] += A[a] * c;
        return B;
    };
    mint d = blackbox_det<mint>(N - 1, apply);
    for (auto& x: outdeg) { d *= fact<mint>(x - 1); }
    return d;
}
} // namespace mitsuha
#endif // AJAY_BEST_THEOREM