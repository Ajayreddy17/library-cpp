#ifndef AJAY_MINIMUM_HAMILTONIAN_CYCLE
#define AJAY_MINIMUM_HAMILTONIAN_CYCLE

#include "library/graph/base.hpp"
#include "library/enumerate/bits.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/minimum_hamiltonian_cycle.hpp

/*
return [cost, cycle]
no cycle: {-1, {}}
*/
template <typename T, typename GT>
pair<T, vector<int>> minimum_hamiltonian_cycle(GT& G) {
    assert(G.is_prepared());
    int n = G.N;
    vector<vector<T>> dist(n, vector<T>(n, infty<T>));
    For(v, n) {
        for (auto&& e : G[v]) chmin(dist[v][e.to], e.cost);
    }
    n -= 1;
    const int full = (1 << n) - 1;
    vector<vector<T>> dp(1 << n, vector<T>(n, infty<T>));
    For(v, n) chmin(dp[1 << v][v], dist[n][v]);
    for (int s = 0; s < (1 << n); ++s) {
        For(frm, n) if (dp[s][frm] < infty<T>) {
            enumerate_all_bit<uint32_t>(full - s, [&](int to) -> void {
                int t = s | 1 << to;
                T cost = dist[frm][to];
                if (cost < infty<T>) chmin(dp[t][to], dp[s][frm] + cost);
            });
        }
    }
    int s = (1 << n) - 1;
    T res = infty<T>;
    int best_v = -1;
    For(v, n) if (dist[v][n] < infty<T> && dp[s][v] < infty<T>) {
        if (chmin(res, dp[s][v] + dist[v][n])) best_v = v;
    }
    if (res == infty<T>) return {-1, {}};
    vector<int> C = {n, best_v};
    int t = s;
    while (len(C) <= n) {
        int to = C.back();
        int frm = [&]() -> int {
            For(frm, n) {
                int s = t ^ (1 << to);
                T inf = infty<T>;
                if (dp[s][frm] < inf && dist[frm][to] < inf &&
                        dp[s][frm] + dist[frm][to] == dp[t][to])
                    return frm;
            }
            return -1;
        }();
        C.emplace_back(frm);
        t ^= 1 << to;
    }
    reverse(C.begin(), C.end());
    return {res, C};
}
} // namespace mitsuha
#endif // AJAY_MINIMUM_HAMILTONIAN_CYCLE
