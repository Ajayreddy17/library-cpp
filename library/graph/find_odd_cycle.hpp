#ifndef AJAY_FIND_ODD_CYCLE
#define AJAY_FIND_ODD_CYCLE

#include "library/graph/strongly_connected_component.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/find_odd_cycle.hpp

// (vs, es), size=(n+1,n)
// https://yukicoder.me/problems/no/1436
template <typename GT>
pair<vector<int>, vector<int>> find_odd_cycle(GT& G) {
    int N = G.N;
    vector<int> comp(N);
    if constexpr (GT::is_directed) {
        comp = strongly_connected_component<GT>(G).second;
    }
    vector<int> dist(2 * N, infty<int>);
    vector<int> par(2 * N, -1); // edge index
    deque<int> que;
    auto add = [&](int v, int d, int p) -> void {
        if (chmin(dist[v], d)) { que.emplace_back(v), par[v] = p; }
    };
    For(root, N) {
        if (dist[2 * root + 0] < infty<int>) continue;
        if (dist[2 * root + 1] < infty<int>) continue;
        add(2 * root, 0, -1);
        while (len(que)) {
            auto v = POP(que);
            auto [a, b] = divmod(v, 2);
            for (auto&& e: G[a]) {
                if (comp[e.frm] != comp[e.to]) continue;
                int w = 2 * e.to + (b ^ 1);
                add(w, dist[v] + 1, e.id);
            }
        }
        if (dist[2 * root + 1] == infty<int>) continue;
        // found
        vector<int> edges;
        vector<int> vs;
        vs.emplace_back(root);
        int v = 2 * root + 1;
        while (par[v] != -1) {
            int i = par[v];
            edges.emplace_back(i);
            auto& e = G.edges[i];
            v = 2 * (e.frm + e.to) + 1 - v;
            vs.emplace_back(v / 2);
        }
        reverse(edges.begin(), edges.end());
        reverse(vs.begin(), vs.end());
        // walk -> cycle
        vector<int> used(N, -1);
        int l = -1, r = -1;
        For(i, len(vs)) {
            if (used[vs[i]] == -1) {
                used[vs[i]] = i;
                continue;
            }
            l = used[vs[i]];
            r = i;
            break;
        }
        assert(l != -1);
        vs = {vs.begin() + l, vs.begin() + r};
        edges = {edges.begin() + l, edges.begin() + r};
        vs.emplace_back(vs[0]);
        return {vs, edges};
    }
    return {};
}
} // namespace mitsuha
#endif // AJAY_FIND_ODD_CYCLE
