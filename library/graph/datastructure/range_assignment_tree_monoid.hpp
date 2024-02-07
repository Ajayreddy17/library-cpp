#ifndef AJAY_RANGE_ASSIGNMENT_TREE_MONOID
#define AJAY_RANGE_ASSIGNMENT_TREE_MONOID

#include "library/monoid/monoid_reverse.hpp"
#include "library/graph/tree.hpp"
#include "library/datastructure/segment_tree/range_assignment_segtree.hpp"

namespace mitsuha{
template <typename TREE, typename Monoid, bool edge>
struct Range_Assignment_Tree_Monoid {
    using MX = Monoid;
    using X = typename MX::value_type;
    TREE &tree;
    int N;
    Range_Assignment_SegTree<MX> seg;
    Range_Assignment_SegTree<Monoid_Reverse<MX>> seg_r;

    Range_Assignment_Tree_Monoid(TREE &tree) : tree(tree), N(tree.N) {
        build([](int i) -> X { return MX::unit(); });
    }
    Range_Assignment_Tree_Monoid(TREE &tree, vector<X> &dat) : tree(tree), N(tree.N) {
        build([&](int i) -> X { return dat[i]; });
    }
    template <typename F>
    Range_Assignment_Tree_Monoid(TREE &tree, F f) : tree(tree), N(tree.N) {
        build(f);
    }
    template <typename F>
    void build(F f) {
        if (!edge) {
            auto f_v = [&](int i) -> X { return f(tree.V[i]); };
            seg.build(N, f_v);
            if constexpr (!MX::commute) { seg_r.build(N, f_v); }
        } else {
            auto f_e = [&](int i) -> X {
                return (i == 0 ? MX::unit() : f(tree.v_to_e(tree.V[i])));
            };
            seg.build(N, f_e);
            if constexpr (!MX::commute) { seg_r.build(N, f_e); }
        }
    }

    void set(int i, X x) {
        if constexpr (edge) i = tree.e_to_v(i);
        i = tree.LID[i];
        seg.assign(i, i + 1, x);
        if constexpr (!MX::commute) seg_r.assign(i, i + 1, x);
    }

    X get(int v) { return seg.prod(tree.LID[v], tree.LID[v] + 1); }

    X prod_path(int u, int v) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto &&[a, b]: pd) { val = MX::op(val, get_prod(a, b)); }
        return val;
    }

    void apply_path(int u, int v, X a) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        for (auto &&[x, y]: pd) {
            int l = min(x, y), r = max(x, y);
            seg.assign(l, r + 1, a);
        }
    }

    void apply_subtree(int u, X a) {
        int l = tree.LID[u], r = tree.RID[u];
        return seg.assign(l + edge, r, a);
    }

    void apply_outtree(int u, X a) {
        int l = tree.LID[u], r = tree.RID[u];
        seg.assign(0 + edge, l + edge, a);
        seg.assign(r, N, a);
    }

    X prod_subtree(int u) {
        int l = tree.LID[u], r = tree.RID[u];
        return seg.prod(l + edge, r);
    }

    X prod_all() { return prod_subtree(tree.V[0]); }

    inline X get_prod(int a, int b) {
        if constexpr (MX::commute) {
            return (a <= b) ? seg.prod(a, b + 1) : seg.prod(b, a + 1);
        }
        return (a <= b) ? seg.prod(a, b + 1) : seg_r.prod(b, a + 1);
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_ASSIGNMENT_TREE_MONOID