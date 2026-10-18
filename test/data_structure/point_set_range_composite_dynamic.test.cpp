#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"
#include <library/template.hpp>

#include <library/datastructure/segment_tree/dynamic_segtree.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;

void solve() {
    using Mono = Monoid_Affine<mint>;
    using X = Mono::value_type;
    LL(N, Q);
    VT(X, dat, N);

    Dynamic_SegTree<Mono, false> seg(0, N);
    auto root = seg.new_node(dat);

    For(Q) {
        LL(t, a, b, c);
        if (t == 0) {
            seg.set(root, a, {b, c});
        }
        if (t == 1) {
            print(Mono::eval(seg.prod(root, a, b), c));
        }
    }
}

signed main() {
    solve();

    return 0;
}
