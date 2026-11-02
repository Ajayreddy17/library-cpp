#ifndef AJAY_COUNT_BICONNECTED_SUBGRAPH
#define AJAY_COUNT_BICONNECTED_SUBGRAPH

#include "library/graph/count/count_connected_subgraph.hpp"
#include "library/setfunc/sps_log.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_biconnected_subgraph.hpp

// O(N^32^N). https://loj.ac/s/2318552.
template <typename T, int LIM>
vector<T> count_biconnected_subgraph(Graph<int, 0> G) {
    int N = G.N;
    auto dp = count_connected_subgraph<T, LIM>(G);
    For(r, N) {
        // r may be art -> r is not art
        vector<T> f(1 << (N - 1));
        For(L, 1 << r) For(R, 1 << (N - 1 - r)) f[L | R << r] = dp[L | (R << (1 + r)) | (1 << r)];
        f = sps_log<T, LIM - 1>(f);
        For(L, 1 << r) For(R, 1 << (N - 1 - r)) dp[L | (R << (1 + r)) | (1 << r)] = f[L | R << r];
    }
    // I see?
    For(v, N) dp[1 << v] = 1;
    return dp;
}
} // namespace mitsuha
#endif // AJAY_COUNT_BICONNECTED_SUBGRAPH
