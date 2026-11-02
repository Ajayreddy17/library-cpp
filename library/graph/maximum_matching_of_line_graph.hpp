#ifndef AJAY_MAXIMUM_MATCHING_OF_LINE_GRAPH
#define AJAY_MAXIMUM_MATCHING_OF_LINE_GRAPH

#include "library/graph/base.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_matching_of_line_graph.hpp

// two edges sharing a vertex can be matched
template <typename GT>
vector<pair<int, int>> maximum_matching_of_line_graph(GT& G) {
    assert(!GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N, M = G.M;
    vector<pair<int, int>> ANS;
    vector<int> V;
    vector<int> par(N, -1); // eid
    {
        vector<int> done(N);
        For(v, N) {
            if (done[v]) continue;
            int cnt = 0;
            auto dfs = [&](auto& dfs, int v, int p) -> void {
                V.emplace_back(v);
                par[v] = p;
                done[v] = 1;
                for (auto&& e: G[v]) {
                    ++cnt;
                    if (done[e.to]) continue;
                    dfs(dfs, e.to, v);
                }
            };
            dfs(dfs, v, -1);
        }
    }
    vector<int> ord(N);
    For(i, N) ord[V[i]] = i;
    vector<int> done(M);
    Frr(i, N) {
        int v = V[i];
        vector<int> down;
        int up = -1;
        for (auto&& e: G[v]) {
            if (done[e.id]) continue;
            if (up == -1 && e.to == par[v]) up = e.id;
            if (ord[e.to] > ord[v]) down.emplace_back(e.id);
        }
        while (len(down) >= 2) {
            auto i = POP(down);
            auto j = POP(down);
            ANS.emplace_back(i, j);
            done[i] = done[j] = 1;
        }
        if (len(down) == 0) continue;
        if (up != -1) {
            int x = up;
            int y = down[0];
            done[x] = done[y] = 1;
            ANS.emplace_back(x, y);
        }
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_MATCHING_OF_LINE_GRAPH
