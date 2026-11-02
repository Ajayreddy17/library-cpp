#ifndef AJAY_INCREMENTAL_SCC
#define AJAY_INCREMENTAL_SCC

#include "library/graph/strongly_connected_component.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/incremental_scc.hpp

// https://codeforces.com/blog/entry/91608
// https://codeforces.com/contest/1989/problem/F
// add edges in order of edge id 0, 1, 2, ...
// for each edge i, the earliest time it lies on a cycle, or infty
// an MST on these times + path max queries gives when two vertices join the same SCC
template <typename GT>
vector<int> incremental_scc(GT& G) {
    static_assert(GT::is_directed);
    int N = G.N, M = G.M;
    vector<int> merge_time(M, infty<int>);
    vector<tuple<int, int, int>> dat;
    For(i, M) {
        auto& e = G.edges[i];
        dat.emplace_back(i, e.frm, e.to);
    }

    vector<int> new_idx(N, -1);
    // not on a cycle at time L, on a cycle at time R
    auto dfs
            = [&](auto& dfs, vector<tuple<int, int, int>>& dat, int L, int R) -> void {
        if (dat.empty() || R == L + 1) return;
        int M = (L + R) / 2;
        int n = 0;
        for (auto& [i, a, b]: dat) {
            if (new_idx[a] == -1) new_idx[a] = n++;
            if (new_idx[b] == -1) new_idx[b] = n++;
        }

        Graph<int, 1> G(n);
        for (auto& [i, a, b]: dat) {
            if (i < M) G.add(new_idx[a], new_idx[b]);
        }
        G.build();
        auto [nc, comp] = strongly_connected_component(G);
        vector<tuple<int, int, int>> dat1, dat2;
        for (auto [i, a, b]: dat) {
            a = new_idx[a], b = new_idx[b];
            if (i < M) {
                if (comp[a] == comp[b]) {
                    chmin(merge_time[i], M), dat1.emplace_back(i, a, b);
                } else {
                    dat2.emplace_back(i, comp[a], comp[b]);
                }
            } else {
                dat2.emplace_back(i, comp[a], comp[b]);
            }
        }
        for (auto& [i, a, b]: dat) new_idx[a] = new_idx[b] = -1;
        dfs(dfs, dat1, L, M), dfs(dfs, dat2, M, R);
    };
    dfs(dfs, dat, 0, M + 1);
    return merge_time;
}
} // namespace mitsuha
#endif // AJAY_INCREMENTAL_SCC
