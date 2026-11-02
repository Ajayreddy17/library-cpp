#ifndef AJAY_COUNT_K4
#define AJAY_COUNT_K4

#include "library/graph/base.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_K4.hpp

// M^{1.5} + M^2/w
// assumes a simple graph
template <typename GT>
long long count_K4(GT& G) {
    static_assert(!GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N;
    Graph<int, 1> DAG(N);
    {
        auto deg = G.deg_array();
        auto comp = [&](int a, int b) -> bool {
            return (deg[a] == deg[b] ? a < b : deg[a] < deg[b]);
        };
        for (auto&& e : G.edges) {
            int a = e.frm, b = e.to;
            if (!comp(a, b)) swap(a, b);
            DAG.add(a, b);
        }
        DAG.build();
    }

    vector<int> new_idx(N, -1);
    long long ANS = 0;
    For(a, N) {
        vector<int> V;
        for (auto&& e : DAG[a]) V.emplace_back(e.to);
        For(i, len(V)) new_idx[V[i]] = i;
        int n = len(V);
        Graph<bool, 1> H(n);
        For(i, n) {
            for (auto&& e : DAG[V[i]]) {
                int j = new_idx[e.to];
                if (j == -1) continue;
                H.add(i, j);
            }
        }
        H.build();
        For(b, cld(n, 64)) {
            int L = 64 * b;
            int R = L + 64;
            chmin(R, n);
            vector<uint64_t> dp(n);
            For(i, L, R) {
                for (auto&& e : H[i]) {
                    dp[e.to] |= uint64_t(1) << (i - L);
                }
            }
            for (auto&& e : H.edges) {
                ANS += popcount(dp[e.frm] & dp[e.to]);
            }
        }
        For(i, len(V)) new_idx[V[i]] = -1;
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_COUNT_K4
