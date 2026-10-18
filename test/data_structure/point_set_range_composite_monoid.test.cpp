#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/datastructure/segment_tree/segtree.hpp>

using mint = modint998;
using Mono = Monoid_Affine<mint>;

void solve() {
    LL(N, Q);
    SegTree<Mono> seg(N, [&](int i) -> pair<mint, mint> {
        INT(a, b);
        return {a, b};
    });
    For(Q) {
        INT(t);
        if (t == 0) {
            INT(i, a, b);
            seg.set(i, {a, b});
        } else {
            INT(L, R, x);
            print(Mono::eval(seg.prod(L, R), mint(x)));
        }
    }
}

signed main() {
    solve();
    return 0;
}
