#ifndef AJAY_ALL_CYCLE_COMMON_VERTICES
#define AJAY_ALL_CYCLE_COMMON_VERTICES

#include "library/graph/connectivity/strongly_connected_component.hpp"
#include "library/graph/connectivity/toposort.hpp"
#include "library/graph/cycle/find_cycle.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/cycle/all_cycle_common_vertices.hpp

// all v (ascending) that lie on some cycle and whose removal makes the graph a DAG
// removing v: non-DAG -> DAG
// probably assumes no self loops
// https://codeforces.com/contest/982/problem/F
template <typename GT>
vector<int> all_cycle_common_vertices(GT& G, bool strongly_connected) {
    static_assert(GT::is_directed);
    int N = G.N;
    if (!strongly_connected) {
        auto [nc, comp] = strongly_connected_component(G);
        vector<int> sz(nc);
        For(v, N) sz[comp[v]]++;
        int k = -1;
        For(i, nc) {
            if (sz[i] >= 2) {
                if (k != -1) return {};
                k = i;
            }
        }
        if (k == -1) return {};  // DAG
        vector<int> V;
        For(v, N) if (comp[v] == k) V.emplace_back(v);
        Graph<int, 1> H = G.rearrange(V);
        vector<int> ANS = all_cycle_common_vertices(H, true);
        for (int& x : ANS) x = V[x];
        return ANS;
    }

    assert(strongly_connected);
    if (N == 1) return {};  // DAG

    // main cycle
    vector<int> C = find_cycle_directed(G).first;

    int n = len(C);
    vector<int> idx(N, -1);
    For(i, n) idx[C[i]] = i;

    vector<int> other;
    For(i, N) if (idx[i] == -1) other.emplace_back(i);
    if (len(other)) {
        Graph<int, 1> H = G.rearrange(other);
        if (toposort(H).empty()) return {};  // two vertex disjoint cycle
    }

    vector<int> F(N + 1);
    auto arc = [&](int s, int t) -> void {
        if (s < t) {
            F[s + 1]++, F[t]--;
        } else {
            F[s + 1]++, F[n]--;
            F[0]++, F[t]--;
        }
    };

    vector<int> dp(N, -2);

    For(s, n) {
        auto eval = [&](int i) -> int {
            if (i < 0) return i;
            return (s < i ? i - s : i + n - s);
        };

        auto dfs = [&](auto& dfs, int v) -> int {
            if (idx[v] != -1) return idx[v];
            if (dp[v] != -2) return dp[v];
            int ans = -1;
            for (auto& e : G[v]) {
                int i = dfs(dfs, e.to);
                if (eval(ans) < eval(i)) ans = i;
            }
            return dp[v] = ans;
        };
        int i = -1;
        for (auto& e : G[C[s]]) {
            int j = dfs(dfs, e.to);
            if (eval(i) < eval(j)) i = j;
        }
        if (i != -1) arc(s, i);
    }
    For(i, n) F[i + 1] += F[i];
    F.pop_back();

    vector<int> ANS;
    For(i, n) if (F[i] == 0) ANS.emplace_back(C[i]);

    if (ANS.empty()) return {};
    vector<int> V;
    For(v, N) if (v != ANS[0]) V.emplace_back(v);
    {
        Graph<int, 1> H = G.rearrange(V);
        if (toposort(H).empty()) return {};
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_ALL_CYCLE_COMMON_VERTICES
