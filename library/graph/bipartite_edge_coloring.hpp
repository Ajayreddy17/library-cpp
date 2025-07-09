#ifndef AJAY_BIPARTITE_EDGE_COLORING
#define AJAY_BIPARTITE_EDGE_COLORING

#include "library/graph/bipartite_vertex_coloring.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/flow/bipartite.hpp"

namespace mitsuha{
struct RegularBipartiteColoring {
    using P = pair<int, int>;
    int N, M;
    vector<P> edges;

    vector<vector<int>> solve(int n, int k, vector<P> G) {
        N = n;
        M = int(G.size());
        edges = G;
        vector<int> A(M);
        iota(A.begin(), A.end(), 0);
        return solve_inner(M / N, A);
    }

    vector<vector<int>> solve_inner(int k, vector<int> A) {
        return (k % 2 == 0 ? solve_even(k, A) : solve_odd(k, A));
    }

    vector<vector<int>> solve_even(int k, vector<int> A) {
        assert(k % 2 == 0);
        if (k == 0) return {};
        // 2^m <= k < 2^{m+1}
        int m = 0;
        while (1 << (m + 1) <= k) ++m;
        vector<vector<int>> res;
        if (k != 1 << m) {
            auto [B, C] = split(k, A);
            auto dat = solve_inner(k / 2, C);
            For(j, k - (1 << m)) res.emplace_back(dat[j]);
            For(j, k - (1 << m), int(dat.size())) {
                for (auto&& idx : dat[j]) B.emplace_back(idx);
            }
            k = 1 << m;
            swap(A, B);
        }
        auto dfs = [&](auto& dfs, int K, vector<int> A) -> void {
            if (K == 1) {
                res.emplace_back(A);
                return;
            }
            auto [B, C] = split(k, A);
            dfs(dfs, K / 2, B);
            dfs(dfs, K / 2, C);
        };
        dfs(dfs, k, A);
        return res;
    }

    vector<vector<int>> solve_odd(int k, vector<int> A) {
        assert(k % 2 == 1);
        if (k == 1) return {A};
        
        vector<bool> match = matching(k, A);
        vector<int> B;
        B.reserve(int(A.size()) - N);
        vector<int> es;
        For(i, int(A.size())) {
            if (match[i]) es.emplace_back(A[i]);
            if (!match[i]) B.emplace_back(A[i]);
        }
        vector<vector<int>> res = solve_inner(k - 1, B);
        res.emplace_back(es);
        return res;
    }

    vector<bool> matching(int k, vector<int> A) {
        Graph<bool, 0> G(N + N);
        vector<int> color(N + N);
        For(v, N) color[v] = 0;
        for (auto&& eid : A) {
            auto [a, b] = edges[eid];
            G.add(a, b);
        }
        G.build();
        BipartiteMatching<decltype(G)> BM(G);
        auto& match = BM.match;
        vector<bool> res(int(A.size()));
        For(i, int(A.size())) {
            auto idx = A[i];
            auto [a, b] = edges[idx];
            if (match[a] == b) {
                match[a] = -1;
                res[i] = 1;
            }
        }
        return res;
    }

    pair<vector<int>, vector<int>> split(int k, vector<int> A) {
        assert(k % 2 == 0);
        // “Divide into two k/2-regular parts.”
        int n = int(A.size());
        vector<bool> rest(n);
        vector<int> A0, A1;
        A0.reserve(n / 2), A1.reserve(n / 2);
        vector<vector<P>> G(N + N);
        For(i, n) {
            rest[i] = 1;
            auto [a, b] = edges[A[i]];
            G[a].emplace_back(i, b);
            G[b].emplace_back(i, a);
        }
        auto dfs = [&](auto& dfs, int v, int color) -> void {
            while (int(G[v].size())) {
                auto [i, to] = G[v].back(); G[v].pop_back();
                if (!rest[i]) continue;
                rest[i] = 0;
                if (color == 0) A0.emplace_back(A[i]);
                if (color == 1) A1.emplace_back(A[i]);
                dfs(dfs, to, 1 ^ color);
            }
        };
        For(v, N) dfs(dfs, v, 0);
        return {A0, A1};
    }
};

template <typename GT>
pair<int, vector<int>> bipartite_edge_coloring(GT& G) {
    if (G.M == 0) return {0, {}};
    
    auto vcolor = bipartite_vertex_coloring<GT>(G);
    auto deg = G.deg_array();
    int D = *max_element(deg.begin(), deg.end());

    UnionFind uf(G.N);
    For(c, 2) {
        min_priority_queue<pair<int, int>> que;
        For(v, G.N) {
            if (vcolor[v] == c) que.emplace(deg[v], v);
        }
        while (int(que.size()) > 1) {
            auto [d1, v1] = que.top(); que.pop();
            auto [d2, v2] = que.top(); que.pop();
            if (d1 + d2 > D) break;
            uf.merge(v1, v2);
            int r = uf[v1];
            que.emplace(d1 + d2, r);
        }
    }

    vector<int> LV, RV;
    For(v, G.N) if (uf[v] == v) {
        if (vcolor[v] == 0) LV.emplace_back(v);
        if (vcolor[v] == 1) RV.emplace_back(v);
    }
    int X = max(int(LV.size()), int(RV.size()));
    vector<int> degL(X), degR(X);

    vector<pair<int, int>> edges;
    for (auto&& e : G.edges) {
        int a = e.frm, b = e.to;
        a = uf[a], b = uf[b];
        a = lower_bound(LV.begin(), LV.end(), a) - LV.begin();
        b = lower_bound(RV.begin(), RV.end(), b) - RV.begin();
        degL[a]++, degR[b]++;
        edges.emplace_back(a, X + b);
    }
    int p = 0, q = 0;
    while(p < X && q < X) {
        if (degL[p] == D) {
            ++p;
            continue;
        }
        if (degR[q] == D) {
            ++q;
            continue;
        }
        edges.emplace_back(p, X + q);
        degL[p]++, degR[q]++;
    }
    RegularBipartiteColoring RBC;
    vector<vector<int>> res = RBC.solve(X, D, edges);
    vector<int> ecolor(int(edges.size()));
    For(i, int(res.size())) {
        for (auto&& j : res[i]) ecolor[j] = i;
    }
    ecolor.resize(G.M);
    return {D, ecolor};
}
} // namespace mitsuha
#endif // AJAY_BIPARTITE_EDGE_COLORING