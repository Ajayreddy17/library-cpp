#ifndef AJAY_LAZY_TREE_MONOID
#define AJAY_LAZY_TREE_MONOID

#include "library/graph/tree.hpp"
#include "library/datastructure/segment_tree/lazy_segtree.hpp"

namespace mitsuha{
template <typename TREE, typename ActedMonoid, bool edge>
struct Lazy_Tree_Monoid {
    using MX = typename ActedMonoid::Monoid_X;
    using MA = typename ActedMonoid::Monoid_A;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    using A = typename MA::value_type;
    TREE &tree;
    int N;
    Lazy_SegTree<ActedMonoid> seg;

    Lazy_Tree_Monoid(TREE &tree) : tree(tree), N(tree.N) {
        build([](int i) -> X { return MX::unit(); });
    }
    Lazy_Tree_Monoid(TREE &tree, vector<X> &dat) : tree(tree), N(tree.N) {
        build([&](int i) -> X { return dat[i]; });
    }
    template <typename F>
    Lazy_Tree_Monoid(TREE &tree, F f) : tree(tree), N(tree.N) {
        build(f);
    }

    template <typename F>
    void build(F f) {
        vector<X> seg_raw(N, MX::unit());
        if (!edge) {
            seg.build(N, [&](int i) -> X { return f(tree.V[i]); });
        } else {
            seg.build(N, [&](int i) -> X {
                return (i == 0 ? MX::unit() : f(tree.v_to_e(tree.V[i])));
            });
        }
    }

    void set(int i, X x) {
        if constexpr (edge) i = tree.e_to_v(i);
        i = tree.LID[i];
        seg.set(i, x);
    }

    X get(int v) { return seg.get(tree.LID[v]); }
    vector<X> get_all() {
        vector<X> dat = seg.get_all();
        if (!edge) {
            vector<X> res(N);
            for(int v = 0; v < N; ++v) res[v] = dat[tree.LID[v]];
            return res;
        } else {
            vector<X> res(N - 1);
            for(int i = 0; i < N - 1; ++i) { res[i] = dat[tree.LID[tree.e_to_v(i)]]; }
            return res;
        }
    }

    X prod_path(int u, int v) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        X val = MX::unit();
        for (auto &&[a, b]: pd) {
            X x = get_prod(a, b);
            val = MX::op(val, x);
        }
        return val;
    }

    X prod_subtree(int u) {
        int l = tree.LID[u], r = tree.RID[u];
        return seg.prod(l + edge, r);
    }

    X prod_all() { return seg.prod_all(); }

    void apply_path(int u, int v, A a) {
        auto pd = tree.get_path_decomposition(u, v, edge);
        for (auto &&[x, y]: pd) {
            int l = min(x, y), r = max(x, y);
            seg.apply(l, r + 1, a);
        }
    }

    void apply_subtree(int u, A a) {
        int l = tree.LID[u], r = tree.RID[u];
        return seg.apply(l + edge, r, a);
    }

    void apply_outtree(int u, A a) {
        int l = tree.LID[u], r = tree.RID[u];
        seg.apply(0 + edge, l + edge, a);
        seg.apply(r, N, a);
    }

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
                auto i = seg.min_left(a + 1, check_tmp);
                if (i == a + 1) return u;
                return tree.V[i];
            }
        }
        return v;
    }
    
    inline X get_prod(int a, int b) {
        return (a <= b ? seg.prod(a, b + 1) : seg.prod(b, a + 1));
    }

private:
    template <class F>
    int max_path_edge(int u, int v, F &check) {
        assert(edge);
        if (!check(MX::unit())) return -1;
        int lca = tree.lca(u, v);
        auto pd = tree.get_path_decomposition(u, lca, edge);
        X val = MX::unit();

        for (auto &&[a, b]: pd) {
            assert(a >= b);
            X x = seg.prod(b, a + 1);
            if (check(MX::op(val, x))) {
                val = MX::op(val, x);
                u = (tree.parent[tree.V[b]]);
                continue;
            }
            auto check_tmp = [&](X x) -> bool { return check(MX::op(val, x)); };
            auto i = seg.min_left(a + 1, check_tmp);
            if (i == a + 1) return u;
            return tree.parent[tree.V[i]];
        }
        pd = tree.get_path_decomposition(lca, v, edge);
        for (auto &&[a, b]: pd) {
            assert(a <= b);
            X x = seg.prod(a, b + 1);
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
#endif // AJAY_LAZY_TREE_MONOID