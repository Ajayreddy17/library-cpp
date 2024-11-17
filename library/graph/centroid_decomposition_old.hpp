#ifndef AJAY_CENTROID_DECOMPOSITION_OLD
#define AJAY_CENTROID_DECOMPOSITION_OLD

#include "library/graph/base.hpp"
#include "library/graph/find_centroid.hpp"

namespace mitsuha{
template <typename GT>
struct Centroid_Decomposition {
    using edge_type = typename GT::edge_type;
    GT& G;
    int N;
    vector<int> sz;
    vector<int> par;
    vector<int> cdep; // depth in centroid tree
    bool calculated;

    Centroid_Decomposition(GT& G) : G(G), N(G.N), sz(G.N), par(G.N), cdep(G.N, -1) {
        calculated = 0;
        build();
    }

  private:
    int find(int v) {
        vector<int> V = {v};
        par[v] = -1;
        int p = 0;
        while (p < len(V)) {
            int v = V[p++];
            sz[v] = 0;
            for (auto&& e : G[v]) {
                if (e.to == par[v] or cdep[e.to] != -1) continue;
                par[e.to] = v;
                V.emplace_back(e.to);
            }
        }
        while (len(V)) {
            int v = V.back();
            V.pop_back();
            sz[v] += 1;
            if (p - sz[v] <= p / 2) return v;
            sz[par[v]] += sz[v];
        }
        return -1;
    }
    void build() {
        assert(G.is_prepared());
        assert(!calculated);
        calculated = 1;

        vector<pair<int, int>> st;
        st.emplace_back(0, 0);
        while (!st.empty()) {
            auto [lv, v] = st.back();
            st.pop_back();
            auto c = find(v);
            cdep[c] = lv;
            for (auto&& e : G[c]) {
                if (cdep[e.to] == -1) {
                    st.emplace_back(lv + 1, e.to);
                }
            }
        }
    }

  public:
    // V, dat, indptr
    template <typename T, typename F>
    tuple<vector<int>, vector<T>, vector<int>> collect_path_data(int root, T root_val, F f) {
        vector<int> V = {root};
        vector<T> dp = {root_val};
        vector<int> indptr = {0, 1};
        for (auto&& e : G[root]) {
            int nxt = e.to;
            if (cdep[nxt] < cdep[root]) continue;
            int p = len(V);
            V.emplace_back(nxt);
            dp.emplace_back(f(root_val, e));
            par[nxt] = root;
            while (p < len(V)) {
                int v = V[p];
                T val = dp[p];
                p++;
                for (auto&& e : G[v]) {
                    if (e.to == par[v]) continue;
                    if (cdep[e.to] < cdep[root]) continue;
                    par[e.to] = v;
                    V.emplace_back(e.to);
                    dp.emplace_back(f(val, e));
                }
            }
            indptr.emplace_back(len(V));
        }
        return {V, dp, indptr};
    }

    // V, dist, indptr
    tuple<vector<int>, vector<int>, vector<int>> collect_dist(int root) {
        auto f = [&](int x, auto e) -> int { return x + 1; };
        return collect_path_data(root, 0, f);
    }

    // (V, H, indptr), (V[i] in G) = (i in H).
    // 0,1,2... is a dfs order in H.
    tuple<vector<int>, Graph<typename GT::cost_type, true>, vector<int>> get_subgraph(int root) {
        static vector<int> conv;
        while (len(conv) < N) conv.emplace_back(-1);

        vector<int> V = {root};
        vector<int> indptr = {0, 1};
        conv[root] = 0;
        using cost_type = typename GT::cost_type;
        vector<tuple<int, int, cost_type>> edges;

        auto dfs = [&](auto& dfs, int v, int p) -> void {
            conv[v] = len(V);
            V.emplace_back(v);
            for (auto&& e : G[v]) {
                int to = e.to;
                if (to == p) continue;
                if (cdep[to] < cdep[root]) continue;
                dfs(dfs, to, v);
                edges.emplace_back(conv[v], conv[to], e.cost);
            }
        };
        for (auto&& e : G[root]) {
            if (cdep[e.to] < cdep[root]) continue;
            dfs(dfs, e.to, root);
            edges.emplace_back(conv[root], conv[e.to], e.cost);
            indptr.emplace_back(len(V));
        }
        int n = len(V);
        Graph<typename GT::cost_type, true> H(n);
        for (auto&& [a, b, c] : edges) H.add(a, b, c);
        H.build();
        for (auto&& v : V) conv[v] = -1;
        return {V, H, indptr};
    }
};
} // namespace mitsuha
#endif // AJAY_CENTROID_DECOMPOSITION_OLD