#ifndef AJAY_TREE
#define AJAY_TREE

#include "library/graph/base.hpp"

namespace mitsuha{
template<typename GT>
struct Tree {
    using Graph_type = GT;
    GT &G;
    using WT = typename GT::cost_type;
    int N;
    vector<int> LID, RID, head, V, parent, VtoE;
    vector<int> depth;
    vector<WT> depth_weighted;

    Tree(GT &G, int r = 0, bool hld = 1) : G(G) { build(r, hld); }

    void build(int r = 0, bool hld = 1) {
        if (r == -1) return;
        N = G.N;
        LID.assign(N, -1), RID.assign(N, -1), head.assign(N, r);
        V.assign(N, -1), parent.assign(N, -1), VtoE.assign(N, -1);
        depth.assign(N, -1), depth_weighted.assign(N, 0);
        assert(G.is_prepared());
        int t1 = 0;
        dfs_sz(r, -1, hld);
        dfs_hld(r, t1);
    }

    void dfs_sz(int v, int p, bool hld) {
        auto &sz = RID;
        parent[v] = p;
        depth[v] = (p == -1 ? 0 : depth[p] + 1);
        sz[v] = 1;
        int l = G.indptr[v], r = G.indptr[v + 1];
        auto &csr = G.csr_edges;
        for (int i = r - 2; i >= l; --i) {
            if (hld && depth[csr[i + 1].to] == -1) swap(csr[i], csr[i + 1]);
        }
        int hld_sz = 0;
        for (int i = l; i < r; ++i) {
            auto e = csr[i];
            if (depth[e.to] != -1) continue;
            depth_weighted[e.to] = depth_weighted[v] + e.cost;
            VtoE[e.to] = e.id;
            dfs_sz(e.to, v, hld);
            sz[v] += sz[e.to];
            if (hld && chmax(hld_sz, sz[e.to]) && l < i) { swap(csr[l], csr[i]); }
        }
    }

    void dfs_hld(int v, int &times) {
        LID[v] = times++;
        RID[v] += LID[v];
        V[LID[v]] = v;
        bool heavy = true;
        for (auto &&e: G[v]) {
            if (depth[e.to] <= depth[v]) continue;
            head[e.to] = (heavy ? head[v] : e.to);
            heavy = false;
            dfs_hld(e.to, times);
        }
    }

    vector<int> heavy_path_at(int v) {
        vector<int> P = {v};
        while (1) {
            int a = P.back();
            for (auto &&e: G[a]) {
                if (e.to != parent[a] && head[e.to] == v) {
                    P.emplace_back(e.to);
                    break;
                }
            }
            if (P.back() == a) break;
        }
        return P;
    }

    int heavy_child(int v) {
        int k = LID[v] + 1;
        if (k == N) return -1;
        int w = V[k];
        return (parent[w] == v ? w : -1);
    }

    int e_to_v(int eid) {
        auto e = G.edges[eid];
        return (parent[e.frm] == e.to ? e.frm : e.to);
    }

    int v_to_e(int v) { return VtoE[v]; }
    int get_eid(int u, int v) {
        if (parent[u] != v) swap(u, v);
        assert(parent[u] == v);
        return VtoE[u];
    }
    int ELID(int v) { return 2 * LID[v] - depth[v]; }
    int ERID(int v) { return 2 * RID[v] - depth[v] - 1; }

    /* k: 0-indexed */
    int la(int v, int k) {
        assert(k <= depth[v]);
        while (1) {
            int u = head[v];
            if (LID[v] - k >= LID[u]) return V[LID[v] - k];
            k -= LID[v] - LID[u] + 1;
            v = parent[u];
        }
    }

    int lca(int u, int v) {
        for (;; v = parent[head[v]]) {
            if (LID[u] > LID[v]) swap(u, v);
            if (head[u] == head[v]) return u;
        }
    }

    int meet(int a, int b, int c) { return lca(a, b) ^ lca(a, c) ^ lca(b, c); }

    int lca_root(int u, int v, int root) {
        return lca(u, v) ^ lca(u, root) ^ lca(v, root);
    }

    int subtree_size(int v, int root = -1) {
        if (root == -1) return RID[v] - LID[v];
        if (v == root) return N;
        int x = jump(v, root, 1);
        if (in_subtree(v, x)) return RID[v] - LID[v];
        return N - RID[x] + LID[x];
    }

    int dist(int a, int b) {
        int c = lca(a, b);
        return depth[a] + depth[b] - 2 * depth[c];
    }

    WT dist_weighted(int a, int b) {
        int c = lca(a, b);
        return depth_weighted[a] + depth_weighted[b] - WT(2) * depth_weighted[c];
    }

    // a is in b
    bool in_subtree(int a, int b) { return LID[b] <= LID[a] && LID[a] < RID[b]; }

    int jump(int a, int b, long long k) {
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

    vector<int> collect_child(int v) {
        vector<int> res;
        for (auto &&e: G[v]) if (e.to != parent[v]) res.emplace_back(e.to);
        return res;
    }

    vector<int> collect_light(int v) {
        vector<int> res;
        bool skip = true;
        for (auto &&e: G[v])
            if (e.to != parent[v]) {
                if (!skip) res.emplace_back(e.to);
                skip = false;
            }
        return res;
    }

    vector<pair<int, int>> get_path_decomposition(int u, int v, bool edge) {
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

    vector<int> restore_path(int u, int v) {
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
    pair<int, int> path_intersection(int a, int b, int c, int d) {
        int ab = lca(a, b), ac = lca(a, c), ad = lca(a, d);
        int bc = lca(b, c), bd = lca(b, d), cd = lca(c, d);
        int x = ab ^ ac ^ bc, y = ab ^ ad ^ bd; // meet(a,b,c), meet(a,b,d)
        if (x != y) return {x, y};
        int z = ac ^ ad ^ cd;
        if (x != z) x = -1;
        return {x, x};
    }
};
} // namespace mitsuha
#endif // AJAY_TREE
