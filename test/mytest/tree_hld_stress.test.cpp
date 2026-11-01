#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/tree.hpp"

// brute force on a rooted tree given by par[]
struct Naive {
    int n, r;
    vector<int> par, dep;
    vector<vector<int>> ch;
    Naive(int n, int r, const vector<pair<int, int>> &E) : n(n), r(r), par(n, -1), dep(n, 0), ch(n) {
        vector<vector<int>> adj(n);
        for (auto [a, b]: E) adj[a].push_back(b), adj[b].push_back(a);
        vector<int> st = {r}, seen(n);
        seen[r] = 1;
        while (!st.empty()) {
            int v = st.back();
            st.pop_back();
            for (int w: adj[v]) if (!seen[w]) seen[w] = 1, par[w] = v, dep[w] = dep[v] + 1, ch[v].push_back(w), st.push_back(w);
        }
    }
    int lca(int a, int b) {
        while (dep[a] > dep[b]) a = par[a];
        while (dep[b] > dep[a]) b = par[b];
        while (a != b) a = par[a], b = par[b];
        return a;
    }
    vector<int> path(int a, int b) {
        int c = lca(a, b);
        vector<int> L, R;
        for (int x = a; x != c; x = par[x]) L.push_back(x);
        L.push_back(c);
        for (int x = b; x != c; x = par[x]) R.push_back(x);
        reverse(R.begin(), R.end());
        L.insert(L.end(), R.begin(), R.end());
        return L;
    }
    int size(int v) {
        int s = 1;
        for (int c: ch[v]) s += size(c);
        return s;
    }
};

template <typename TR>
vector<int> expand(TR &T, int u, int v, bool edge) {
    vector<int> P;
    for (auto [a, b]: T.get_path_decomposition(u, v, edge)) {
        if (a <= b) For(i, a, b + 1) P.push_back(T.V[i]);
        else Frr(i, b, a + 1) P.push_back(T.V[i]);
    }
    return P;
}

void test_random() {
    For(iter, 2000) {
        int n = RNG(1, 40), r = RNG(0, n);
        int shape = RNG(0, 3);
        vector<pair<int, int>> E;
        For(i, 1, n) {
            int p = (shape == 0 ? RNG(0, i) : shape == 1 ? i - 1 : (RNG(0, 3) ? 0 : RNG(0, i)));
            E.emplace_back(p, i);
        }
        vector<int> perm(n);
        iota(perm.begin(), perm.end(), 0);
        For(i, n) swap(perm[i], perm[RNG(0, i + 1)]);
        for (auto &[a, b]: E) a = perm[a], b = perm[b];
        bool hld = RNG(0, 4) != 0;
        Graph<int, 0> G(n);
        for (auto [a, b]: E) G.add(a, b);
        G.build();
        Tree<decltype(G)> T(G, r, hld);
        Naive B(n, r, E);

        For(v, n) {
            assert(T.parent[v] == B.par[v] && T.depth[v] == B.dep[v]);
            assert(T.V[T.LID[v]] == v && T.RID[v] - T.LID[v] == B.size(v));
            vector<int> c1 = T.collect_child(v), c2 = B.ch[v];
            sort(c1.begin(), c1.end()), sort(c2.begin(), c2.end());
            assert(c1 == c2);
            if (hld) {
                int h = T.heavy_child(v);
                for (int c: c2) assert(h != -1 && B.size(c) <= B.size(h));
                if (h != -1) assert(G[v].begin()->to == h);
                int t = T.tail(v);
                assert(T.head[t] == T.head[v] && T.heavy_child(t) == -1);
                if (T.head[v] == v) {
                    auto hp = T.heavy_path_at(v);
                    assert(hp.front() == v && hp.back() == t);
                }
            }
        }
        // Euler tour: subtree(v) = [ELID, ERID)
        vector<int> tour;
        auto dfs = [&](auto &self, int v) -> void {
            tour.push_back(v);
            for (int c: T.collect_child(v)) self(self, c), tour.push_back(v);
        };
        dfs(dfs, r);
        For(v, n) {
            int f = -1, l = -1;
            For(i, len(tour)) if (tour[i] == v) { if (f < 0) f = i; l = i; }
            assert(f == T.ELID(v) && l + 1 == T.ERID(v));
        }

        For(q, 50) {
            int u = RNG(0, n), v = RNG(0, n), w = RNG(0, n);
            auto P = B.path(u, v);
            assert(T.lca(u, v) == B.lca(u, v));
            assert(T.dist(u, v) == len(P) - 1);
            assert(T.meet(u, v, w) == (B.lca(u, v) ^ B.lca(u, w) ^ B.lca(v, w)));
            int k = RNG(0, T.depth[u] + 1);
            int x = u;
            For(k) x = B.par[x];
            assert(T.la(u, k) == x);
            long long kk = RNG(0, n + 2);
            assert(T.jump(u, v, kk) == (kk < len(P) ? P[kk] : -1));
            assert(expand(T, u, v, false) == P);
            vector<int> PE(P);
            PE.erase(find(PE.begin(), PE.end(), B.lca(u, v)));
            assert(expand(T, u, v, true) == PE);
            assert(T.restore_path(u, v) == P);
            // subtree_size(u) when rooted at w
            Naive C(n, w, E);
            assert(T.subtree_size(u, w) == C.size(u));
            // max_path with a monotone check: within distance d of u
            int d = RNG(0, n);
            assert(T.max_path([&](int y) { return T.dist(u, y) <= d; }, u, v) == P[min(d, len(P) - 1)]);
        }
    }
}

void test_deep_path() {
    int n = 300000;
    Graph<int, 0> G(n);
    For(i, 1, n) G.add(i - 1, i);
    G.build();
    Tree<decltype(G)> T(G);
    assert(T.lca(n - 1, n / 2) == n / 2);
    assert(T.la(n - 1, n - 1) == 0);
    assert(T.subtree_size(0) == n);
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test_random();
    test_deep_path();
    solve();
    return 0;
}
