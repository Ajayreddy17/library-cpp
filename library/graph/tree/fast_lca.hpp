#ifndef AJAY_FAST_LCA
#define AJAY_FAST_LCA

#include "library/graph/tree.hpp"
#include "library/monoid/monoid_min.hpp"
#include "library/datastructure/sparse_table/sparse_table.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/fast_lca.hpp
template <typename TREE>
struct Fast_Lca {
    TREE& tree;
    Sparse_Table<Monoid_Min<int>> seg;
    vector<int> pos;

    Fast_Lca(TREE& tree) : tree(tree) {
        int N = tree.N;
        pos.resize(N);
        vector<int> dat(2 * N);
        For(v, N) {
            int a = tree.ELID(v);
            int b = tree.ERID(v);
            pos[v] = a;
            dat[a] = tree.LID[v];
            dat[b] = (v == tree.V[0] ? -1 : tree.LID[tree.parent[v]]);
        }
        seg.build(dat);
    }

    int dist(int a, int b) {
        int c = lca(a, b);
        return tree.depth[a] + tree.depth[b] - 2 * tree.depth[c];
    }

    using WT = typename TREE::WT;
    WT dist_weighted(int a, int b) {
        int c = lca(a, b);
        return tree.depth_weighted[a] + tree.depth_weighted[b] - WT(2) * tree.depth_weighted[c];
    }

    int lca(int a, int b) {
        int p = pos[a], q = pos[b];
        if (p > q) swap(p, q);
        return tree.V[seg.prod(p, q + 1)];
    }
};
} // namespace mitsuha
#endif // AJAY_FAST_LCA