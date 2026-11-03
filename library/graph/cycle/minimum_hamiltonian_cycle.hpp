#ifndef AJAY_MINIMUM_HAMILTONIAN_CYCLE
#define AJAY_MINIMUM_HAMILTONIAN_CYCLE

#include "library/graph/base.hpp"
#include "library/enumerate/bits.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/cycle/minimum_hamiltonian_cycle.hpp

/*
return [cost, cycle]
no cycle: {-1, {}}
*/
template <typename T, typename GT>
pair<T, vector<int>> minimum_hamiltonian_cycle(GT& G) {
    assert(G.is_prepared());
    int n = G.N;
    vector<vector<T>> dist(n, vector<T>(n, numeric_limits<T>::max() / 2));
    For(v, n) {
        for (auto&& e : G[v]) chmin(dist[v][e.to], e.cost);
    }
    n -= 1;
    const int full = (1 << n) - 1;
    vector<vector<T>> dp(1 << n, vector<T>(n, numeric_limits<T>::max() / 2));
    For(v, n) chmin(dp[1 << v][v], dist[n][v]);
    for (int s = 0; s < (1 << n); ++s) {
        For(frm, n) if (dp[s][frm] < numeric_limits<T>::max() / 2) {
            enumerate_all_bit<uint32_t>(full - s, [&](int to) -> void {
                int t = s | 1 << to;
                T cost = dist[frm][to];
                if (cost < numeric_limits<T>::max() / 2) chmin(dp[t][to], dp[s][frm] + cost);
            });
        }
    }
    int s = (1 << n) - 1;
    T res = numeric_limits<T>::max() / 2;
    int best_v = -1;
    For(v, n) if (dist[v][n] < numeric_limits<T>::max() / 2 && dp[s][v] < numeric_limits<T>::max() / 2) {
        if (chmin(res, dp[s][v] + dist[v][n])) best_v = v;
    }
    if (res == numeric_limits<T>::max() / 2) return {-1, {}};
    vector<int> C = {n, best_v};
    int t = s;
    while (len(C) <= n) {
        int to = C.back();
        int frm = [&]() -> int {
            For(frm, n) {
                int s = t ^ (1 << to);
                T inf = numeric_limits<T>::max() / 2;
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
