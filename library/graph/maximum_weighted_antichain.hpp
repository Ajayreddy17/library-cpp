#ifndef AJAY_MAXIMUM_WEIGHTED_ANTICHAIN
#define AJAY_MAXIMUM_WEIGHTED_ANTICHAIN

#include "library/flow/maxflow.hpp"
#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_weighted_antichain.hpp

template <typename WT, typename GT>
pair<WT, vector<int>> maximum_weighted_antichain(GT& G, vector<WT> wt) {
    int N = G.N;
    int s = 2 * N, t = 2 * N + 1;
    MaxFlow<WT> F(2 * N + 2, s, t);
    For(i, N) F.add(s, 2 * i + 0, wt[i]);
    For(i, N) F.add(2 * i + 1, t, wt[i]);
    for (auto& e: G.edges) F.add(2 * e.frm + 0, 2 * e.to + 1, infty<WT>);
    auto [ans, cut] = F.cut();
    vector<int> antichain;
    For(v, N) {
        if (cut[2 * v + 0] == 0 && cut[2 * v + 1] == 1) antichain.emplace_back(v);
    }
    ans = accumulate(wt.begin(), wt.end(), WT(0)) - ans;
    return {ans, antichain};
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_WEIGHTED_ANTICHAIN
