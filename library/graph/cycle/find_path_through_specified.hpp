#ifndef AJAY_FIND_PATH_THROUGH_SPECIFIED
#define AJAY_FIND_PATH_THROUGH_SPECIFIED

#include "library/graph/base.hpp"
#include "library/random/base.hpp"
#include "library/number/GF2.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/cycle/find_path_through_specified.hpp

// minimum length of an (s,t) path visiting every vertex in need
// s = t is allowed. Also returns the first step.
// {length, first v}; {-1,-1} if none
// O(2^K(N+M)L), L = shortest path length
template <typename GT>
pair<int, int> find_path_through_specified(GT& G, int s, int t, vector<int> need) {
    static_assert(!GT::is_directed);
    int N = G.N;
    assert(0 <= s && s < N && 0 <= t && t < N);
    using F = GF2<64>;
    // frm, to, wt
    vector<tuple<int, int, F>> edges;
    vector<int> S, T;
    for (auto& e: G.edges) {
        int a = e.frm, b = e.to;
        if ((a == s && b == t) || (a == t && b == s)) {
            if (need.empty()) return {1, t};
            continue;
        }
        if (a == s || b == s) { S.emplace_back(a ^ b ^ s); }
        if (a == t || b == t) { T.emplace_back(a ^ b ^ t); }
        if (a != s && a != t && b != s && b != t) {
            F x = RNG_64();
            edges.emplace_back(a, b, x), edges.emplace_back(b, a, x);
        }
    }
    int K = len(need);
    vector<int> get(N);
    For(k, K) get[need[k]] = 1 << k;
    int M = len(edges);

    vector<vector<F>> dp_e(1 << K, vector<F>(M));
    vector<vector<F>> dp_v(1 << K, vector<F>(N));
    for (auto& v: T) dp_v[get[v]][v] = RNG_64();
    For(L, 1, N) {
        for (auto& v: S) {
            if (dp_v.back()[v] != F(0)) return {1 + L, v};
        }
        vector<vector<F>> newdp_e(1 << K, vector<F>(M));
        vector<vector<F>> newdp_v(1 << K, vector<F>(N));
        For(s, 1 << K) {
            For(m, M) {
                auto [a, b, c] = edges[m];
                int t = s | get[b];
                if (get[b] && s == t) continue;
                F x = (dp_v[s][a] + dp_e[s][m ^ 1]) * c;
                newdp_e[t][m] += x, newdp_v[t][b] += x;
            }
        }
        swap(dp_v, newdp_v), swap(dp_e, newdp_e);
    }
    return {-1, -1};
}
} // namespace mitsuha
#endif // AJAY_FIND_PATH_THROUGH_SPECIFIED
