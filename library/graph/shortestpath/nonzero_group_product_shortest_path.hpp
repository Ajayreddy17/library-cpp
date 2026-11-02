#ifndef AJAY_NONZERO_GROUP_PRODUCT_SHORTEST_PATH
#define AJAY_NONZERO_GROUP_PRODUCT_SHORTEST_PATH

#include "library/graph/base.hpp"
#include "library/graph/shortestpath/dijkstra.hpp"
#include "library/util/pop.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/shortest_path/nonzero_group_product_shortest_path.hpp


// given: group-labeled undirected graph G, and s.
// return: shortest path length from s to v, for each v.
// remark: path is not a walk. directed case is NP hard.
// https://arxiv.org/abs/1906.04062
template <typename WT, typename Monoid, typename GT>
vector<WT> nonzero_group_product_shortest_path(
        GT& G, vector<typename Monoid::value_type> edge_label, int s) {
    static_assert(!GT::is_directed);
    const int N = G.N, M = G.M;

    using X = typename Monoid::value_type;

    auto get = [&](int eid, int frm) -> X {
        auto& e = G.edges[eid];
        X x = edge_label[e.id];
        return (e.frm == frm ? x : Monoid::inverse(x));
    };

    // shortest path tree
    vector<WT> dist(N, infty<WT>);
    vector<X> phi(N, Monoid::unit());
    vector<int> par(N, -1);
    vector<int> depth(N);
    dist[s] = 0;
    min_priority_queue<pair<WT, int>> que;
    que.emplace(0, s);
    while (len(que)) {
        auto [dv, v] = POP(que);
        if (dv != dist[v]) continue;
        for (auto& e : G[v]) {
            if (chmin(dist[e.to], dv + e.cost)) {
                phi[e.to] = Monoid::op(phi[v], get(e.id, v));
                que.emplace(dist[e.to], e.to);
                par[e.to] = v, depth[e.to] = depth[v] + 1;
            }
        }
    }

    vector<bool> cons(M);
    For(i, M) {
        int a = G.edges[i].frm, b = G.edges[i].to;
        X x = Monoid::op(phi[a], edge_label[i]);
        cons[i] = (x == phi[b]);
    }

    vector<WT> h(M, infty<WT>);
    vector<WT> q(N, infty<WT>);
    for (auto& e : G.edges) {
        if (dist[e.frm] == infty<WT>) continue;
        if (!cons[e.id] && chmin(h[e.id], dist[e.frm] + dist[e.to] + e.cost)) {
            que.emplace(h[e.id], e.id);
        }
    }

    vector<int> root(N);
    For(v, N) root[v] = v;

    auto get_root = [&](int v) -> int {
        while (root[v] != v) {
            v = root[v] = root[root[v]];
        }
        return v;
    };

    while (len(que)) {
        auto [x, eid] = POP(que);
        if (x != h[eid]) continue;
        int a = G.edges[eid].frm, b = G.edges[eid].to;
        a = get_root(a), b = get_root(b);
        vector<int> B;
        while (a != b) {
            if (depth[a] < depth[b]) swap(a, b);
            B.emplace_back(a), a = get_root(par[a]);
        }
        for (auto& w : B) {
            root[w] = a, q[w] = x - dist[w];
            for (auto& e : G[w]) {
                if (cons[e.id] && chmin(h[e.id], q[w] + dist[e.to] + e.cost)) {
                    que.emplace(h[e.id], e.id);
                }
            }
        }
    }

    vector<WT> ANS(N, infty<WT>);
    For(v, N) ANS[v] = (phi[v] == Monoid::unit() ? q[v] : dist[v]);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_NONZERO_GROUP_PRODUCT_SHORTEST_PATH
