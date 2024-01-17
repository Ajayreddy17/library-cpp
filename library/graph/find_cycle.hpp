#ifndef AJAY_FIND_CYCLE
#define AJAY_FIND_CYCLE

#include "library/graph/base.hpp"

namespace mitsuha{
template <typename GT>
pair<vector<int>, vector<int>> find_cycle_directed(GT& G) {
    static_assert(GT::is_directed());
    assert(G.is_prepared());
    int N = G.N;
    vector<int> used(N);
    vector<pair<int, int>> par(N);
    vector<int> es, vs;

    auto dfs = [&](auto self, int v) -> void {
        used[v] = 1;
        for (auto&& e: G[v]) {
            if (len(es)) return;
            if (!used[e.to]) {
                par[e.to] = {v, e.id};
                self(self, e.to);
            }
            elif (used[e.to] == 1) {
                es = {e.id};
                int cur = v;
                while (cur != e.to) {
                    es.emplace_back(par[cur].second);
                    cur = par[cur].first;
                }
                reverse(es.begin(), es.end());
                return;
            }
        }
        used[v] = 2;
    };
    for(int v = 0; v < N; v++) if (!used[v]) dfs(dfs, v);
    if (es.empty()) return {vs, es};

    vs.resize(len(es));
    for(int i = 0; i < len(es); i++) { vs[i] = G.edges[es[i]].frm; }
    return {vs, es};
}

template <typename GT>
pair<vector<int>, vector<int>> find_cycle_undirected(GT& G) {
    assert(!GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N;
    const int M = G.M;
    vector<int> dep(N, -1);
    vector<bool> used_e(M);
    vector<int> par(N, -1);

    auto dfs = [&](auto& dfs, int v, int d) -> int {
        dep[v] = d;
        for (auto&& e: G[v]) {
            if (used_e[e.id]) continue;
            if (dep[e.to] != -1) return e.id;
            used_e[e.id] = 1;
            par[e.to] = e.id;
            int res = dfs(dfs, e.to, d + 1);
            if (res != -1) return res;
        }
        return -1;
    };

    vector<int> vs, es;
    for(int v = 0; v < N; v++) {
        if (dep[v] != -1) continue;
        int e0 = dfs(dfs, v, 0);
        if (e0 == -1) continue;
        int a = G.edges[e0].frm, b = G.edges[e0].to;
        if (dep[a] > dep[b]) swap(a, b);
        es.emplace_back(e0);
        vs.emplace_back(a);
        while (1) {
            int x = vs.back();
            auto& e = G.edges[es.back()];
            int y = e.frm + e.to - x;
            if (y == a) break;
            vs.emplace_back(y);
            es.emplace_back(par[y]);
        }
        return {vs, es};
    }
    return {vs, es};
}

// {vs, es} ：Returns a sequence of edges and a sequence of vertices. es[i] is vs[i] to vs[i+1].
template <typename GT>
pair<vector<int>, vector<int>> find_cycle(GT& G) {
    if (G.is_directed()) return find_cycle_directed(G);
    return find_cycle_undirected(G);
}
} // namespace mitsuha
#endif // AJAY_FIND_CYCLE
