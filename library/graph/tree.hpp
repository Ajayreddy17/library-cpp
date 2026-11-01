#ifndef AJAY_TREE
#define AJAY_TREE

#include "library/graph/base.hpp"

namespace mitsuha{
// HLD order (heavy child first) when hld = true; otherwise preorder in edge order.
// Building reorders G's adjacency lists so that G[v][0] is the heavy child.
template<typename GT>
struct Tree {
    using Graph_type = GT;
    GT &G;
    using WT = typename GT::cost_type;
    int N;
    vector<int> LID, RID, head, V, parent, VtoE, EtoV;
    vector<int> depth;
    vector<WT> depth_weighted;
    vector<int> memo_tail;

    Tree(GT &G, int r = 0, bool hld = 1) : G(G) { build(r, hld); }

    void build(int r = 0, bool hld = 1) {
        if (r == -1) return;
        N = G.N;
        LID.assign(N, -1), RID.assign(N, -1), head.assign(N, r);
        V.assign(N, -1), parent.assign(N, -1), VtoE.assign(N, -1), EtoV.assign(G.M, -1);
        depth.assign(N, -1), depth_weighted.assign(N, 0);
        memo_tail.clear();
        assert(G.is_prepared());
        auto &csr = G.csr_edges;

        // 1st pass: parent, depth, subtree size (kept in RID for now)
        vector<int> order, st = {r};
        order.reserve(N);
        depth[r] = 0;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            order.emplace_back(v);
            for (int i = G.indptr[v]; i < G.indptr[v + 1]; ++i) {
                auto &e = csr[i];
                if (depth[e.to] != -1) continue;
                parent[e.to] = v, depth[e.to] = depth[v] + 1;
                depth_weighted[e.to] = depth_weighted[v] + e.cost;
                VtoE[e.to] = e.id, EtoV[e.id] = e.to;
                st.emplace_back(e.to);
            }
        }
        auto &sz = RID;
        for (int i = (int)order.size() - 1; i >= 0; --i) {
            int v = order[i];
            sz[v] = 1;
            for (int j = G.indptr[v]; j < G.indptr[v + 1]; ++j) {
                if (is_child_edge(v, csr[j])) sz[v] += sz[csr[j].to];
            }
        }
        // move the heavy child to the front of G[v]
        if (hld) {
            for (int v: order) {
                int best = -1;
                for (int j = G.indptr[v]; j < G.indptr[v + 1]; ++j) {
                    if (!is_child_edge(v, csr[j])) continue;
                    if (best == -1 || sz[csr[j].to] > sz[csr[best].to]) best = j;
                }
                if (best != -1) rotate(csr.begin() + G.indptr[v], csr.begin() + best, csr.begin() + best + 1);
            }
        }

        // 2nd pass: preorder, first child continues the path
        int t = 0;
        st = {r};
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            LID[v] = t, V[t] = v, ++t;
            RID[v] += LID[v];
            int first = -1;
            for (int j = G.indptr[v + 1] - 1; j >= G.indptr[v]; --j) {
                if (!is_child_edge(v, csr[j])) continue;
                int c = csr[j].to;
                head[c] = c, first = c;
                st.emplace_back(c);
            }
            if (first != -1) head[first] = head[v];
        }
    }

    template <typename E>
    bool is_child_edge(int v, const E &e) const { return parent[e.to] == v && VtoE[e.to] == e.id; }

    // vertices of the heavy path whose top is h, top to bottom
    vector<int> heavy_path_at(int h) const {
        assert(head[h] == h);
        vector<int> P;
        for (int k = LID[h]; k < N && head[V[k]] == h; ++k) P.emplace_back(V[k]);
        return P;
    }

    int heavy_child(int v) const {
        if (RID[v] == LID[v] + 1) return -1;
        return V[LID[v] + 1];
    }

    // bottom vertex of v's heavy path
    int tail(int v) {
        if (memo_tail.empty()) {
            memo_tail.assign(N, -1);
            for (int i = N - 1; i >= 0; --i) {
                int u = V[i], w = heavy_child(u);
                memo_tail[u] = (w == -1 ? u : memo_tail[w]);
            }
        }
        return memo_tail[v];
    }

    int e_to_v(int eid) const {
        if (EtoV[eid] != -1) return EtoV[eid];
        auto e = G.edges[eid];
        return (parent[e.frm] == e.to ? e.frm : e.to);
    }

    int v_to_e(int v) const { return VtoE[v]; }
    int get_eid(int u, int v) const {
        if (parent[u] != v) swap(u, v);
        assert(parent[u] == v);
        return VtoE[u];
    }
    int ELID(int v) const { return 2 * LID[v] - depth[v]; }
    int ERID(int v) const { return 2 * RID[v] - depth[v] - 1; }

    /* k: 0-indexed */
    int la(int v, int k) const {
        assert(k <= depth[v]);
        while (1) {
            int u = head[v];
            if (LID[v] - k >= LID[u]) return V[LID[v] - k];
            k -= LID[v] - LID[u] + 1;
            v = parent[u];
        }
    }

    int lca(int u, int v) const {
        for (;; v = parent[head[v]]) {
            if (LID[u] > LID[v]) swap(u, v);
            if (head[u] == head[v]) return u;
        }
    }

    int meet(int a, int b, int c) const { return lca(a, b) ^ lca(a, c) ^ lca(b, c); }

    int lca_root(int u, int v, int root) const {
        return lca(u, v) ^ lca(u, root) ^ lca(v, root);
    }

    int subtree_size(int v, int root = -1) const {
        if (root == -1) return RID[v] - LID[v];
        if (v == root) return N;
        int x = jump(v, root, 1);
        if (in_subtree(v, x)) return RID[v] - LID[v];
        return N - RID[x] + LID[x];
    }

    int dist(int a, int b) const {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }

    WT dist_weighted(int a, int b) const {
        int c = lca(a, b);
        return depth_weighted[a] + depth_weighted[b] - WT(2) * depth_weighted[c];
    }

    // a is in b
    bool in_subtree(int a, int b) const { return LID[b] <= LID[a] && LID[a] < RID[b]; }

    int jump(int a, int b, long long k) const {
        if (k == 1) {
            if (a == b) return -1;
            return (in_subtree(b, a) ? la(b, depth[b] - depth[a] - 1) : parent[a]);
        }
        int c = lca(a, b);
        int d_ac = depth[a] - depth[c];
        int d_bc = depth[b] - depth[c];
        if (k > d_ac + d_bc) return -1;
        if (k <= d_ac) return la(a, k);
        return la(b, d_ac + d_bc - k);
    }

    vector<int> collect_child(int v) const {
        vector<int> res;
        for (int k = LID[v] + 1; k < RID[v]; k = RID[V[k]]) res.emplace_back(V[k]);
        return res;
    }
    void collect_child(int v, vector<int> &buf) const {
        buf.clear();
        for (int k = LID[v] + 1; k < RID[v]; k = RID[V[k]]) buf.emplace_back(V[k]);
    }

    template <typename F>
    void for_each_child(int v, const F &f) const {
        for (int k = LID[v] + 1; k < RID[v]; k = RID[V[k]]) f(V[k]);
    }

    vector<int> collect_subtree(int v) const { return {V.begin() + LID[v], V.begin() + RID[v]}; }

    vector<int> collect_light(int v) const {
        vector<int> res;
        for (int k = LID[v] + 1; k < RID[v]; k = RID[V[k]]) if (head[V[k]] == V[k]) {
            res.emplace_back(V[k]);
        }
        return res;
    }

    // closed LID ranges [a, b] in walking order; a > b means walking up
    vector<pair<int, int>> get_path_decomposition(int u, int v, bool edge) const {
        vector<pair<int, int>> up, down;
        while (1) {
            if (head[u] == head[v]) break;
            if (LID[u] < LID[v]) {
                down.emplace_back(LID[head[v]], LID[v]);
                v = parent[head[v]];
            } else {
                up.emplace_back(LID[u], LID[head[u]]);
                u = parent[head[u]];
            }
        }
        if (LID[u] < LID[v]) down.emplace_back(LID[u] + edge, LID[v]);
        else if (LID[v] + edge <= LID[u]) up.emplace_back(LID[u], LID[v] + edge);
        reverse(down.begin(), down.end());
        up.insert(up.end(), down.begin(), down.end());
        return up;
    }

    // edge runs (frm, to, kind), kind in "heavy_up", "heavy_down", "light_up", "light_down"
    vector<tuple<int, int, string>> get_path_decomposition_detail(int u, int v) const {
        vector<tuple<int, int, string>> up, down;
        while (1) {
            if (head[u] == head[v]) break;
            if (LID[u] < LID[v]) {
                if (v != head[v]) down.emplace_back(head[v], v, "heavy_down"), v = head[v];
                down.emplace_back(parent[v], v, "light_down"), v = parent[v];
            } else {
                if (u != head[u]) up.emplace_back(u, head[u], "heavy_up"), u = head[u];
                up.emplace_back(u, parent[u], "light_up"), u = parent[u];
            }
        }
        if (LID[u] < LID[v]) down.emplace_back(u, v, "heavy_down");
        else if (LID[v] < LID[u]) up.emplace_back(u, v, "heavy_up");
        reverse(down.begin(), down.end());
        up.insert(up.end(), down.begin(), down.end());
        return up;
    }

    vector<int> restore_path(int u, int v) const {
        vector<int> P;
        for (auto &&[a, b]: get_path_decomposition(u, v, 0)) {
            if (a <= b) for (int i = a; i <= b; i++) P.emplace_back(V[i]);
            else for (int i = a; i >= b; i--) P.emplace_back(V[i]);
        }
        return P;
    }

    // path intersection of [a, b] x [c, d]
    // {-1, -1} if there wasn't any intersection
    // https://codeforces.com/problemset/problem/500/G
    pair<int, int> path_intersection(int a, int b, int c, int d) const {
        int ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
        int bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
        int x = ab ^ ac ^ bc, y = ab ^ ad ^ bd; // meet(a,b,c), meet(a,b,d)
        if (x != y) return {x, y};
        int z = ac ^ ad ^ cd;
        if (x != z) x = -1;
        return {x, x};
    }

    // last x on the u-v path with check(x) true (check must be monotone along the path);
    // -1 if check(u) is false
    template <class F>
    int max_path(F check, int u, int v) const {
        if (!check(u)) return -1;
        for (auto [a, b]: get_path_decomposition(u, v, false)) {
            if (!check(V[a])) return u;
            if (check(V[b])) {
                u = V[b];
                continue;
            }
            int ok = a, ng = b; // check(V[ok]) true, check(V[ng]) false
            while (abs(ok - ng) > 1) {
                int x = (ok + ng) / 2;
                (check(V[x]) ? ok : ng) = x;
            }
            return V[ok];
        }
        return u;
    }
};
} // namespace mitsuha
#endif // AJAY_TREE
