#ifndef AJAY_TOPOSORT
#define AJAY_TOPOSORT

#include "library/graph/base.hpp"
#include "library/datastructure/fastset.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/toposort.hpp

// lexicographically smallest toposort
template <typename GT>
vector<int> toposort(GT& G) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N;
    auto [indeg, outdeg] = G.deg_array_inout();
    FastSet que(N);
    vector<int> V;
    For(v, N) if (indeg[v] == 0) que.insert(v);
    while (1) {
        int v = que.next(0);
        if (v == N) break;
        que.erase(v), V.emplace_back(v);
        for (auto&& e: G[v]) {
            if (--indeg[e.to] == 0) que.insert(e.to);
        }
    }
    return (len(V) < N ? vector<int>{} : V);
}

// inv_perm=true: the inverse permutation is lexicographically smallest (positions of 0, 1, ... are minimal)
template <typename GT>
vector<int> lex_min_toposort(GT& G, bool inv_perm = false) {
    static_assert(GT::is_directed);
    assert(G.is_prepared());
    const int N = G.N;
    if (inv_perm) {
        GT H(N);
        for (auto& e: G.edges) H.add(N - 1 - e.to, N - 1 - e.frm);
        H.build();
        auto V = lex_min_toposort(H, false);
        reverse(V.begin(), V.end());
        for (auto& x: V) x = N - 1 - x;
        return V;
    }
    auto [indeg, outdeg] = G.deg_array_inout();
    FastSet que(N);
    vector<int> V;
    For(v, N) if (indeg[v] == 0) que.insert(v);
    while (1) {
        int v = que.next(0);
        if (v == N) break;
        que.erase(v), V.emplace_back(v);
        for (auto&& e: G[v]) {
            if (--indeg[e.to] == 0) que.insert(e.to);
        }
    }
    return (len(V) < N ? vector<int>{} : V);
}
} // namespace mitsuha
#endif // AJAY_TOPOSORT
