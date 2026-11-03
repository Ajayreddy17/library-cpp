#ifndef AJAY_SCC
#define AJAY_SCC

#include "library/graph/base.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/connectivity/strongly_connected_component.hpp

template <typename GT>
pair<int, vector<int>> strongly_connected_component(const GT& G) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    int N = G.N;
    int nc = 0;
    vector<int> comp(N), low(N), ord(N, -1), path;
    vector<int> par(N, -1), it = G.indptr;
    int now = 0;

    For(s, N) {
        if (ord[s] != -1) continue;
        int v = s;
        low[v] = ord[v] = now++;
        path.emplace_back(v);

        while (v != -1) {
            if (it[v] < G.indptr[v + 1]) {
                int to = G.csr_edges[it[v]++].to;
                if (ord[to] == -1) {
                    par[to] = v;
                    v = to;
                    low[v] = ord[v] = now++;
                    path.emplace_back(v);
                } else {
                    chmin(low[v], ord[to]);
                }
            } else {
                if (low[v] == ord[v]) {
                    while (1) {
                        int u = POP(path);
                        ord[u] = N, comp[u] = nc;
                        if (u == v) break;
                    }
                    ++nc;
                }
                int p = par[v];
                if (p != -1) chmin(low[p], low[v]);
                v = p;
            }
        }
    }
    For(v, N) comp[v] = nc - 1 - comp[v];
    return {nc, comp};
}

template <typename GT>
Graph<int, 1> scc_dag(const GT& G, int nc, const vector<int>& comp) {
    Graph<int, 1> DAG(nc);
    vector<vector<int>> edges(nc);
    for (auto&& e : G.edges) {
        int x = comp[e.frm], y = comp[e.to];
        if (x == y) continue;
        edges[x].emplace_back(y);
    }
    For(c, nc) {
        settify(edges[c]);
        for (auto&& to : edges[c]) DAG.add(c, to);
    }
    DAG.build();
    return DAG;
}
} // namespace mitsuha
#endif // AJAY_SCC
