#ifndef AJAY_RANGE_ASSIGNMENT_SEGTREE
#define AJAY_RANGE_ASSIGNMENT_SEGTREE

#include "library/datastructure/fastset.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_pow.hpp"

namespace mitsuha{
template <typename Monoid>
struct Range_Assignment_SegTree {
    using MX = Monoid;
    using X = typename MX::value_type;
    int n;
    SegTree<MX> seg;
    FastSet cut;
    vector<X> dat;

    Range_Assignment_SegTree() {}
    Range_Assignment_SegTree(int n) { build(n); }
    template <typename F>
    Range_Assignment_SegTree(int n, F f) {
        build(n, f);
    }
    Range_Assignment_SegTree(const vector<X> &v) { build(v); }

    void build(int m) {
        build(m, [](int i) -> X { return MX::unit(); });
    }
    void build(const vector<X> &v) {
        build(len(v), [&](int i) -> X { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        n = m;
        seg.build(m, f), cut.build(n, [&](int i) -> int { return 1; });
        dat = seg.get_all();
    }

    X prod(int l, int r) {
        int a = cut.prev(l), b = cut.next(l), c = cut.prev(r);
        if (a == c) { return monoid_pow<MX>(dat[a], r - l); };
        assert(b <= c);
        X x = monoid_pow<MX>(dat[a], b - l);
        X y = seg.prod(b, c);
        X z = monoid_pow<MX>(dat[c], r - c);
        return MX::op(MX::op(x, y), z);
    }

    void assign(int l, int r, X x) {
        int a = cut.prev(l), b = cut.next(r);
        if (a < l) seg.set(a, monoid_pow<MX>(dat[a], l - a));
        if (r < b) {
            X y = dat[cut.prev(r)];
            dat[r] = y, cut.insert(r), seg.set(r, monoid_pow<MX>(y, b - r));
        }
        cut.enumerate(l + 1, r, [&](int i) -> void { seg.set(i, MX::unit()), cut.erase(i); });
        dat[l] = x, cut.insert(l), seg.set(l, monoid_pow<MX>(x, r - l));
    }
};
} // namespace mitsuha
#endif // AJAY_RANGE_ASSIGNMENT_SEGTREE