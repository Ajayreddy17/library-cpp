#ifndef AJAY_FIND_C4
#define AJAY_FIND_C4

#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/cycle/find_C4.hpp

// find one C4 in an undirected graph
// https://codeforces.com/problemset/problem/1468/M
template <typename GT>
tuple<int, int, int, int> find_C4(GT& G) {
    static_assert(!GT::is_directed);
    int N = G.N;
    auto deg = G.deg_array();
    auto I = argsort(deg);
    reverse(I.begin(), I.end());
    vector<int> rk(N);
    For(i, N) rk[I[i]] = i;

    // sort successors in decreasing order
    vector<vector<int>> TO(N);
    for (auto&& e: G.edges) {
        int a = rk[e.frm], b = rk[e.to];
        TO[a].emplace_back(b);
        TO[b].emplace_back(a);
    }
    For(v, N) {
        sort(TO[v].begin(), TO[v].end());
        reverse(TO[v].begin(), TO[v].end());
    }
    vector<int> par(N, -1);
    For(a, N) {
        for (auto&& b: TO[a]) TO[b].pop_back();
        for (auto&& b: TO[a]) {
            for (auto&& c: TO[b]) {
                if (par[c] != -1) { return {I[a], I[b], I[c], I[par[c]]}; }
                par[c] = b;
            }
        }
        for (auto&& b: TO[a]) {
            for (auto&& c: TO[b]) { par[c] = -1; }
        }
    }
    return {-1, -1, -1, -1};
}
} // namespace mitsuha
#endif // AJAY_FIND_C4
