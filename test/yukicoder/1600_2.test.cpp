#define PROBLEM "https://yukicoder.me/problems/no/1600"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/sparse_table/sparse_table.hpp"
#include "library/monoid/monoid_min.hpp"
#include "library/graph/tree.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/mod/modint.hpp"

using mint = modint107;

void solve() {
    LL(N, M);
    Graph<mint, 0> G(N);
    UnionFind uf(N);
    vector<pair<int, int>> edges;
    vector<bool> in_G(M);
    mint wt = 1;
    For(i, M) {
        LL(a, b);
        --a, --b;
        edges.emplace_back(a, b);
        wt += wt;
        if (uf.merge(a, b)) {
            in_G[i] = 1;
            G.add(a, b, wt);
        }
    }
    G.build();
    Tree<decltype(G)> tree(G);
    auto& par = tree.parent;
    // non-tree edge e -> point (LID[a], LID[b]) with weight e
    vector<int> X, Y, W;
    For(e, M) {
        if (in_G[e]) continue;
        auto [a, b] = edges[e];
        a = tree.LID[a], b = tree.LID[b];
        if (a > b) swap(a, b);
        X.emplace_back(a), Y.emplace_back(b), W.emplace_back(e);
    }
    using Mono = Monoid_Min<int>;
    Wavelet_Matrix_2D_Range<int, false, false, Sparse_Table<Mono>> seg(
        len(X), [&](int i) -> tuple<int, int, int> { return {X[i], Y[i], W[i]}; });
    LL(Q);
    For(Q) {
        LL(u, v, idx);
        --u, --v, --idx;
        auto [x, y] = edges[idx];
        if (par[y] == x) swap(x, y);
        bool in_u = tree.in_subtree(u, x);
        bool in_v = tree.in_subtree(v, x);
        if (!in_G[idx] || in_u == in_v) {
            print(tree.dist_weighted(u, v));
            continue;
        }
        // the walk must leave the subtree of x through a non-tree edge
        int l = tree.LID[x], r = tree.RID[x];
        int min_i = Mono::op(seg.prod(0, l, l, r), seg.prod(l, r, r, N));
        if (min_i == Mono::unit()) {
            print(-1);
            continue;
        }
        auto [p, q] = edges[min_i];
        bool in_p = tree.in_subtree(p, x);
        if (!in_u) swap(u, v);
        if (!in_p) swap(p, q);
        mint ANS = tree.dist_weighted(u, p) + tree.dist_weighted(v, q);
        ANS += mint(2).pow(min_i + 1);
        print(ANS);
    }
}

signed main() {
    solve();
    return 0;
}
