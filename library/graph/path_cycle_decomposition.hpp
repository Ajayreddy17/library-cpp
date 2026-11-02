#ifndef AJAY_PATH_CYCLE_DECOMPOSITION
#define AJAY_PATH_CYCLE_DECOMPOSITION

#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/path_cycle_decomposition.hpp

// for a graph where every vertex has degree <= 2,
// split into vertex lists of paths and of cycles
//
template <typename GT>
pair<vector<vector<int>>, vector<vector<int>>> path_cycle_decomposition(GT& G) {
    static_assert(!GT::is_directed);
    int N = G.N;
    auto deg = G.deg_array();
    assert(*max_element(deg.begin(), deg.end()) <= 2);

    vector<bool> done(N);
    auto calc_frm = [&](int v) -> vector<int> {
        vector<int> P = {v};
        done[v] = 1;
        while (1) {
            bool ok = 0;
            for (auto&& e : G[P.back()]) {
                if (done[e.to]) continue;
                P.emplace_back(e.to);
                done[e.to] = 1;
                ok = 1;
                break;
            }
            if (!ok) break;
        }
        return P;
    };
    vector<vector<int>> paths, cycs;
    For(v, N) {
        if (deg[v] == 0) {
            done[v] = 1;
            paths.emplace_back(vector<int>({int(v)}));
        }
        if (done[v] || deg[v] != 1) continue;
        paths.emplace_back(calc_frm(v));
    }
    For(v, N) {
        if (done[v]) continue;
        cycs.emplace_back(calc_frm(v));
    }
    return {paths, cycs};
}
} // namespace mitsuha
#endif // AJAY_PATH_CYCLE_DECOMPOSITION
