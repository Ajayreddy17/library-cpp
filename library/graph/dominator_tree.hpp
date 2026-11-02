#ifndef AJAY_DOMINATOR_TREE
#define AJAY_DOMINATOR_TREE

#include "library/graph/reverse_graph.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/dominator_tree.hpp

// returns the parent (idom) of each vertex; -1 if unreachable
// https://codeforces.com/contest/757/problem/F
template <typename GT>
vector<int> dominator_tree(GT& G, int root) {
    static_assert(GT::is_directed);
    const int N = G.N;

    vector<int> par(N, -1), ord(N, -1), V;
    V.reserve(N);
    auto dfs = [&](auto& dfs, int v) -> void {
        ord[v] = len(V);
        V.emplace_back(v);
        for (auto&& e: G[v]) {
            if (ord[e.to] == -1) par[e.to] = v, dfs(dfs, e.to);
        }
    };
    dfs(dfs, root);

    auto RG = reverse_graph(G);

    vector<int> sdom(N, -1);
    For(v, N) sdom[v] = v;
    vector<int> mi = sdom;
    vector<int> anc(N, -1);
    vector<int> us(N);

    auto find = [&](auto& find, int v) -> int {
        auto& a = anc[v];
        if (a == -1) return v;
        int b = find(find, a);
        if (ord[sdom[mi[a]]] < ord[sdom[mi[v]]]) mi[v] = mi[a];
        return anc[v] = b;
    };

    vector<vector<int>> bucket(N);

    Frr(i, 1, len(V)) {
        int w = V[i];
        for (auto&& e: RG[w]) {
            int v = e.to;
            if (ord[v] == -1) continue;
            find(find, v);
            if (ord[sdom[mi[v]]] < ord[sdom[w]]) sdom[w] = sdom[mi[v]];
        }
        bucket[sdom[w]].emplace_back(w);

        for (auto&& v: bucket[par[w]]) { find(find, v), us[v] = mi[v]; }
        bucket[par[w]].clear();
        anc[w] = par[w];
    }
    vector<int> idom(N, -1);
    For(i, 1, len(V)) {
        int w = V[i];
        idom[w] = (sdom[w] == sdom[us[w]] ? sdom[w] : idom[us[w]]);
    }
    return idom;
}
} // namespace mitsuha
#endif // AJAY_DOMINATOR_TREE
