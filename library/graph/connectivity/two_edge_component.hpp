#ifndef AJAY_TWO_EDGE_COMPONENT
#define AJAY_TWO_EDGE_COMPONENT

#include "library/graph/base.hpp"

namespace mitsuha{
// (number of components, vector of component numbers)
template <typename GT>
pair<int, vector<int>> two_edge_component(GT& G) {
    static_assert(!GT::is_directed);
    int N = G.N, M = G.M, n_comp = 0;
    vector<int> V, par(N, -2), dp(N), comp(N);
    V.reserve(N);
    vector<bool> used(M);
    auto dfs = [&](auto& dfs, int v) -> void {
        V.emplace_back(v);
        for (auto&& e: G[v]) {
            if (used[e.id]) continue;
            if (par[e.to] != -2) dp[v]++, dp[e.to]--, used[e.id] = 1;
            if (par[e.to] == -2) {
                used[e.id] = 1;
                par[e.to] = v;
                dfs(dfs, e.to);
            }
        }
    };
    For(v, N) if (par[v] == -2) { par[v] = -1, dfs(dfs, v); }
    Frr(i, N) {
        if (par[V[i]] != -1) dp[par[V[i]]] += dp[V[i]];
    }
    for (auto&& v: V) comp[v] = (dp[v] == 0 ? n_comp++ : comp[par[v]]);
    return {n_comp, comp};
}
} // namespace mitsuha
#endif // AJAY_TWO_EDGE_COMPONENT