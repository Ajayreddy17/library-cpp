#ifndef AJAY_UNICYCLICGRAPH
#define AJAY_UNICYCLICGRAPH

#include "library/graph/tree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// https://x.com/maspy_stars/status/1559115993909043206
// graph must be undirected
// use TO[root] to find cut edge, out_cost is the cost of cut edge
// out_eid also available
// G is the spanning tree without the out edge, directed parent -> child, rooted at root;
// tree refers to G, so the struct is not copyable. Edge i of G is edge orig_eid[i] of the input.
template <typename GT>
struct UnicyclicGraph {
    static_assert(!GT::is_directed);
    using T = typename GT::cost_type;
    GT &G0;
    int N;
    int root;
    int out_eid;
    T out_cost;
    vector<int> TO;
    vector<int> cycle;     // Vertex sequence towards the root
    vector<bool> in_cycle; // vertex id -> bool
    vector<int> nearest_in_cycle; // cycle vertex that v's hanging tree attaches to
    Graph<T, 1> G;
    Tree<Graph<T, 1>> tree;
    vector<int> orig_eid;

    UnicyclicGraph(GT &G0) : G0(G0), N(G0.N), tree(G, -1) {
        assert(N == G0.M);
        UnionFind uf(N);
        TO.assign(N, -1);
        For(eid, N) {
            auto &e = G0.edges[eid];
            if (uf.merge(e.frm, e.to)) continue;
            out_eid = eid, out_cost = e.cost;
            root = e.frm;
            TO[root] = e.to;
            break;
        }
        vector<bool> done(N);
        vector<int> que = {root};
        while (len(que)) {
            int v = que.back();
            que.pop_back();
            done[v] = 1;
            for (auto &&e: G0[v]) {
                if (done[e.to] || e.id == out_eid) continue;
                TO[e.to] = v;
                que.emplace_back(e.to);
            }
        }
        cycle = {TO[root]};
        while (cycle.back() != root) cycle.emplace_back(TO[cycle.back()]);
        in_cycle.assign(N, 0);
        for (auto &&v: cycle) in_cycle[v] = 1;

        G.build(N);
        For(eid, N) {
            if (eid == out_eid) continue;
            auto &e = G0.edges[eid];
            int a = e.frm, b = e.to;
            if (a != root && TO[a] == b) swap(a, b); // TO[root] is the removed edge
            assert(b != root && TO[b] == a);
            G.add(a, b, e.cost);
            orig_eid.emplace_back(eid);
        }
        G.build();
        tree.build(root);
        nearest_in_cycle.assign(N, -1);
        for (int v: tree.V) nearest_in_cycle[v] = (in_cycle[v] ? v : nearest_in_cycle[tree.parent[v]]);
    }
    UnicyclicGraph(const UnicyclicGraph &) = delete;
    UnicyclicGraph &operator=(const UnicyclicGraph &) = delete;

    int dist(int a, int b) {
        int ra = nearest_in_cycle[a], rb = nearest_in_cycle[b];
        if (ra == rb) return tree.dist(a, b); // same hanging tree
        int d = abs(tree.depth[ra] - tree.depth[rb]);
        d = min<int>(d, len(cycle) - d);
        return d + tree.depth[a] + tree.depth[b] - tree.depth[ra] - tree.depth[rb];
    }

    T dist_weighted(int a, int b) {
        int btm = TO[root];
        int ra = nearest_in_cycle[a], rb = nearest_in_cycle[b];
        if (ra == rb) return tree.dist_weighted(a, b);
        vector<T> &D = tree.depth_weighted;
        T d = abs(D[ra] - D[rb]);
        d = min(d, D[btm] + out_cost - d);
        return d + D[a] + D[b] - D[ra] - D[rb];
    }
};
} // namespace mitsuha
#endif // AJAY_UNICYCLICGRAPH