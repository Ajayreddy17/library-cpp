#ifndef AJAY_STEINER_TREE
#define AJAY_STEINER_TREE

#include "library/graph/base.hpp"
#include "library/util/subset_iter.hpp"

namespace mitsuha{
// // Edge weights are given by e.cost, and vertex weights are passed as a vector.
// returns {cost, vs, es}
// O(3^k n + 2^k (n + m) log n), k: subset_size
template <typename T, typename GT>
tuple<T, vector<int>, vector<int>> steiner_tree(GT& G, vector<int> S, vector<T> v_wt = {}) {
    static_assert(!GT::is_directed);
    assert(not S.empty());
    const int N = G.N, M = G.M, K = len(S);
    if (v_wt.empty()) v_wt.assign(N, 0);

    // Terminal set, root -> cost
    vector<vector<T>> DP(1 << K, vector<T>(N, numeric_limits<T>::max() / 2));
    For(v, N) DP[0][v] = v_wt[v];
    // 2 * t or 2 * eid + 1
    vector<vector<int>> par(1 << K, vector<int>(N, -1));

    for (int s = 1; s < (1 << K); ++s) {
        auto& dp = DP[s];
        for (int k : all_setbit(s)) {
            int v = S[k];
            chmin(dp[v], DP[s ^ 1 << k][v]);
        }
        for (unsigned int t : all_subset(s)) {
            if (t == 0 || t == s) continue;
            For(v, N) {
                if (chmin(dp[v], DP[t][v] + DP[s ^ t][v] - v_wt[v])) par[s][v] = 2 * t;
            }
        }
        // Move the root using Dijkstra’s algorithm
        min_priority_queue<pair<T, int>> que;
        For(v, N) que.emplace(dp[v], v);
        while (!que.empty()) {
            auto [dv, v] = que.top(); que.pop();
            if (dv != dp[v]) continue;
            for (auto&& e : G[v]) {
                if (chmin(dp[e.to], dv + e.cost + v_wt[e.to])) {
                    par[s][e.to] = 2 * e.id + 1;
                    que.emplace(dp[e.to], e.to);
                }
            }
        }
    }

    vector<bool> used_v(N), used_e(M);
    vector<int> v_to_k(N, -1);
    For(k, K) v_to_k[S[k]] = k;

    vector<pair<int, int>> que;
    int root = min_element((DP.back()).begin(), (DP.back()).end()) - DP.back().begin();
    que.emplace_back((1 << K) - 1, root);
    used_v[root] = 1;

    while (len(que)) {
        auto [s, v] = que.back(); que.pop_back();
        if (s == 0) continue; 
        if (par[s][v] == -1) {
            int k = v_to_k[v];
            assert(k != -1 && s >> k & 1);
            que.emplace_back(s ^ 1 << k, v);
            continue;
        }
        elif (par[s][v] & 1) {
            int eid = par[s][v] / 2;
            auto& e = G.edges[eid];
            int w = v ^ e.frm ^ e.to;
            used_v[w] = used_e[eid] = 1;
            que.emplace_back(s, w);
            continue;
        }
        else {
            int t = par[s][v] / 2;
            que.emplace_back(t, v), que.emplace_back(s ^ t, v);
        }
    }
    vector<int> vs, es;
    For(v, N) if (used_v[v]) vs.emplace_back(v);
    For(e, M) if (used_e[e]) es.emplace_back(e);
    T cost = 0;
    for (auto&& v : vs) cost += v_wt[v];
    for (auto&& e : es) cost += G.edges[e].cost;
    assert(cost == DP.back()[root]);
    return {cost, vs, es};
}
} // namespace mitsuha
#endif // AJAY_STEINER_TREE