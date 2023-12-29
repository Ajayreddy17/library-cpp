#ifndef AJAY_SCC
#define AJAY_SCC

#include "library/graph/base.hpp"

namespace mitsuha{
template <typename GT>
pair<int, vector<int>> strongly_connected_component(GT& G) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    int N = G.N;
    int C = 0;
    vector<int> comp(N), low(N), ord(N, -1), visited;
    int now = 0;

    auto dfs = [&](auto& dfs, int v) -> void {
        low[v] = ord[v] = now++;
        visited.emplace_back(v);
        for (auto &&[frm, to, cost, id]: G[v]) {
            if (ord[to] == -1) {
                dfs(dfs, to);
                low[v] = min(low[v], low[to]);
            } else {
                low[v] = min(low[v], ord[to]);
            }
        }
        if (low[v] == ord[v]) {
            while (1) {
                int u = visited.back();
                visited.pop_back();
                ord[u] = N;
                comp[u] = C;
                if (u == v) break;
            }
            ++C;
        }
    };
    for (int v = 0; v < N; v++) {
        if (ord[v] == -1) dfs(dfs, v);
    }
    for (int v = 0; v < N; v++) comp[v] = C - 1 - comp[v];
    return {C, comp};
}

template <typename GT>
Graph<int, 1> scc_dag(GT& G, int C, vector<int>& comp) {
    Graph<int, 1> DAG(C);
    vector<vector<int>> edges(C);
    for (auto&& e: G.edges) {
        int x = comp[e.frm], y = comp[e.to];
        if (x == y) continue;
        edges[x].emplace_back(y);
    }
    for(int c = 0; c < C; c++) {
        sort(edges[c].begin(), edges[c].end());
        edges[c].erase(unique(edges[c].begin(), edges[c].end()), edges[c].end());
        for (auto&& to: edges[c]) DAG.add(c, to);
    }
    DAG.build();
    return DAG;
}
} // namespace mitsuha
#endif // AJAY_SCC