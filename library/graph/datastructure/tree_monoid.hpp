#ifndef AJAY_TREE_MONOID
#define AJAY_TREE_MONOID

#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_reverse.hpp"
#include "library/graph/tree.hpp"

namespace mitsuha{
template <typename TREE, typename Monoid, bool edge>
struct Tree_Monoid {
    using MX = Monoid;
    using X = typename MX::value_type;
    TREE &tree;
    int N;
    SegTree<MX> seg;
    SegTree<Monoid_Reverse<MX>> seg_r;

    Tree_Monoid(TREE &tree) : tree(tree), N(tree.N) {
        build([](int i) -> X { return MX::unit(); });
    }
    Tree_Monoid(TREE &tree, vector<X> &dat) : tree(tree), N(tree.N) {
        build([&](int i) -> X { return dat[i]; });
    }
    template <typename F>
    Tree_Monoid(TREE &tree, F f) : tree(tree), N(tree.N) {
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
        seg.set(i, x);
        if constexpr (!MX::commute) seg_r.set(i, x);
    }

    void multiply(int i, X x) {
        if constexpr (edge) i = tree.e_to_v(i);
        i = tree.LID[i];
        seg.multiply(i, x);
        if constexpr (!MX::commute) seg_r.multiply(i, x);
    }

    X prod_path(int u, int v) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto &&[a, b]: pd) { val = MX::op(val, get_prod(a, b)); }
        return val;
    }

    // Last x on uv path where prod_path(u, x) satisfies check
    // If not (i.e. path(u,u) is ng) -1
    template <class F>
    int max_path(int u, int v, F &check) {
        if constexpr (edge) return max_path_edge(check, u, v);
        if (!check(prod_path(u, u))) return -1;
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto &&[a, b]: pd) {
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.V[b]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            if (a <= b) {
                auto i = seg.max_right(a, check_tmp);
                return (i == a ? u : tree.V[i - 1]);
            } else {
                int i = 0;
                if constexpr (MX::commute) i = seg.min_left(a + 1, check_tmp);
                if constexpr (!MX::commute) i = seg_r.min_left(a + 1, check_tmp);
                if (i == a + 1) return u;
                return tree.V[i];
            }
        }
        return v;
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

private:
    template <class F>
    int max_path_edge(int u, int v, F &check) {
        static_assert(edge);
        if (!check(MX::unit())) return -1;
        int lca = tree.lca(u, v);
        auto pd = tree.get_path_decomposition(u, lca, edge);
        X val = MX::unit();

        for (auto &&[a, b]: pd) {
            assert(a >= b);
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.parent[tree.V[b]]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            int i = 0;
            if constexpr (MX::commute) i = seg.min_left(a + 1, check_tmp);
            if constexpr (!MX::commute) i = seg_r.min_left(a + 1, check_tmp);
            if (i == a + 1) return u;
            return tree.parent[tree.V[i]];
        }
        pd = tree.get_path_decomposition(lca, v, edge);
        for (auto &&[a, b]: pd) {
            assert(a <= b);
            X x = get_prod(a, b);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.V[b]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            auto i = seg.max_right(a, check_tmp);
            return (i == a ? u : tree.V[i - 1]);
        }
        return v;
    }
};
} // namespace mitsuha
#endif // AJAY_TREE_MONOID