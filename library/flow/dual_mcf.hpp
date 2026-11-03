#ifndef AJAY_DUAL_MCF
#define AJAY_DUAL_MCF

#include "library/flow/bflow.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/flow/dual_mcf.hpp

// overflow with INF edges is not handled
// minimize sum b[v]p[v] + sum_{uv}c[uv]max(0, -p[u]+p[v]-w[uv]))
// sum_v b[v] = 0, c[uv] >= 0
struct Dual_MCF {
    MinCostFlow<long long, long long> G;
    Dual_MCF(int N) : G(N) {}

    // b[v]p[v]
    void add_b(int v, long long b) { G.add_source(v, b); }

    // c[uv]max(0, -p[u]+p[v]-w)
    void add_c(int u, int v, long long w, long long c) { G.add(u, v, 0, c, w); }

    // -p[u]+p[v]<=w
    void add_constraint(int u, int v, int w, long long inf = iinf) {
        add_c(u, v, w, inf);
    }

    // min_val, p
    // if ng: {numeric_limits<__int128>::max() / 2, {}}
    pair<__int128, vector<long long>> solve() {
        auto [ok, val] = G.solve();
        if (!ok) return {numeric_limits<__int128>::max() / 2, {}};
        auto P = G.get_potential();
        return {-val, P};
    }
};
} // namespace mitsuha
#endif // AJAY_DUAL_MCF
