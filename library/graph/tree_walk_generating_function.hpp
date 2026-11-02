#ifndef AJAY_TREE_WALK_GENERATING_FUNCTION
#define AJAY_TREE_WALK_GENERATING_FUNCTION

#include "library/graph/shortestpath/bfs01.hpp"
#include "library/graph/characteristic_poly_of_tree_adjacency_matrix.hpp"
#include "library/poly/convolution_all.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_walk_generating_function.hpp


// without loops: 1600ms (N=10^5)
// with loops: 3300ms (N=10^5)
template <bool ALLOW_LOOP, typename mint, typename F>
pair<vector<mint>, vector<mint>> tree_walk_generating_function(Graph<int, 0>& G, int s,
                                                                                                              int t, F weight) {
    int N = G.N;
    // denominator
    auto f =
            characteristic_poly_of_tree_adjacency_matrix<ALLOW_LOOP, mint>(G, weight);
    // numerator
    // product of components along the (s,t) path with sign adjustment + other components
    using poly = vector<mint>;
    vector<poly> polys;
    pair<int, mint> path_poly = {0, mint(1)};
    vector<bool> on_path(N);
    auto [dist, par] = bfs01<int>(G, s);
    on_path[t] = 1;
    while (t != s) {
        mint w = weight(par[t], t);
        t = par[t], on_path[t] = 1;
        path_poly.first += 1, path_poly.second *= w;  // +wx
    }
    UnionFind uf(N);
    for (auto& e : G.edges) {
        if (on_path[e.frm] || on_path[e.to]) continue;
        uf.merge(e.frm, e.to);
    }
    vector<vector<int>> comp(N);
    For(v, N) comp[uf[v]].emplace_back(v);
    For(r, N) {
        if (on_path[r] || uf[r] != r) continue;
        vector<int>& V = comp[r];
        Graph<int, 0> H = G.rearrange(V);
        poly f = characteristic_poly_of_tree_adjacency_matrix<ALLOW_LOOP, mint>(
                H, [&](int i, int j) -> mint { return weight(V[i], V[j]); });
        polys.emplace_back(f);
    }
    poly B = convolution_all<mint>(polys);
    int m = path_poly.first;
    poly g(len(B) + m);
    For(i, len(B)) g[m + i] = path_poly.second * B[i];
    return {g, f};
}
} // namespace mitsuha
#endif // AJAY_TREE_WALK_GENERATING_FUNCTION
