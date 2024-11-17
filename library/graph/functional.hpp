#ifndef AJAY_FUNCTIONAL
#define AJAY_FUNCTIONAL

#include "library/graph/tree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

namespace mitsuha{
// https://x.com/maspy_stars/status/1559842309688008707 
template <typename T = int>
struct FunctionalGraph {
    int N, M;
    vector<int> TO;
    vector<T> wt;
    vector<int> root;
    Graph<T, 1> G;

    FunctionalGraph() {}
    FunctionalGraph(int N) : N(N), M(0), TO(N, -1), wt(N), root(N, -1) {}

    void add(int a, int b, T c = 1) {
        Assert(0 <= a && a < N);
        Assert(TO[a] == -1);
        ++M;
        TO[a] = b;
        wt[a] = c;
    }

    pair<Graph<T, 1>, Tree<Graph<T, 1>>> build() {
        Assert(N == M);
        UnionFind uf(N);
        For(v, N) if (!uf.merge(v, TO[v])) { root[v] = v; }
        For(v, N) if (root[v] == v) root[uf[v]] = v;
        For(v, N) root[v] = root[uf[v]];

        G.build(N + 1);
        For(v, N) {
            if (root[v] == v)
                G.add(N, v, wt[v]);
            else
                G.add(TO[v], v, wt[v]);
        }
        G.build();
        Tree<Graph<T, 1>> tree(G, N);
        return {G, tree};
    }

    // a to b steps, T_inf if not possible
    template <typename TREE>
    int dist(TREE& tree, int a, int b) {
        if (tree.in_subtree(a, b)) return tree.depth[a] - tree.depth[b];
        int r = root[a];
        int btm = TO[r];
        // a -> r -> btm -> b
        if (tree.in_subtree(btm, b)) {
            int x = tree.depth[a] - tree.depth[r];
            x += 1;
            x += tree.depth[btm] - tree.depth[b];
            return x;
        }
        return numeric_limits<T>::max() / 2;
    }

    template <typename TREE>
    int jump(TREE& tree, int v, long long step) {
        int d = tree.depth[v];
        if (step <= d - 1) return tree.jump(v, N, step);
        v = root[v];
        step -= d - 1;
        int bottom = TO[v];
        int c = tree.depth[bottom];
        step %= c;
        if (step == 0) return v;
        return tree.jump(bottom, N, step - 1);
    }

    template <typename TREE>
    vector<int> jump_all(TREE& tree, long long step) {
        vector<int> res(N, -1);
        vector<vector<pair<int, int>>> query(N);
        For(v, N) {
            int d = tree.depth[v];
            int r = root[v];
            if (d - 1 > step) { query[v].emplace_back(v, step); }
            if (d - 1 <= step) {
                long long k = step - (d - 1);
                int bottom = TO[r];
                int c = tree.depth[bottom];
                k %= c;
                if (k == 0) {
                    res[v] = r;
                    continue;
                }
                query[bottom].emplace_back(v, k - 1);
            }
        }

        vector<int> path;
        auto dfs = [&](auto& dfs, int v) -> void {
            path.emplace_back(v);
            for (auto&& [w, k]: query[v]) { res[w] = path[len(path) - 1 - k]; }
            for (auto&& e: G[v]) dfs(dfs, e.to);
            path.pop_back();
        };
        for (auto&& e: G[N]) { dfs(dfs, e.to); }
        return res;
    }

    template <typename TREE>
    bool in_cycle(TREE& tree, int v) {
        int r = root[v];
        int bottom = TO[r];
        return tree.in_subtree(bottom, v);
    }

    vector<int> collect_cycle(int r) {
        assert(r == root[r]);
        vector<int> cyc = {TO[r]};
        while (cyc.back() != r) cyc.emplace_back(TO[cyc.back()]);
        return cyc;
    }

    // smallest k such that F^k(i) = F^k(j), 
    // or return -1 if no such k exist
    template <typename TREE>
    int meet_time(TREE& tree, int i, int j) {
        if (i == j) return 0;
        if (root[i] != root[j]) return -1;
        int r = root[i];
        int b = TO[r];
        int n = tree.depth[b] - tree.depth[r] + 1; // cyc len
        if ((tree.depth[i] - tree.depth[j]) % n != 0) return -1;
        if (tree.depth[i] == tree.depth[j]) {
            int lca = tree.lca(i, j);
            return tree.depth[i] - tree.depth[lca];
        }
        int ti = tree.depth[i] - tree.depth[tree.lca(b, i)];
        int tj = tree.depth[j] - tree.depth[tree.lca(b, j)];
        return max(ti, tj);
    }
};
} // namespace mitsuha
#endif // AJAY_FUNCTIONAL