#define PROBLEM "https://judge.yosupo.jp/problem/range_set_range_composite"

#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/datastructure/segment_tree/range_assignment_segtree.hpp>

using mint = modint998;
using Mono = Monoid_Affine<mint>;
using AFF = typename Mono::value_type;

void solve() {
    INT(N, Q);

    Range_Assignment_SegTree<Mono> seg(N, [&](int i) -> AFF {
        INT(a, b);
        return {mint(a), mint(b)};
    });

    For(Q) {
        INT(t);
        if (t == 0) {
            INT(l, r, b, c);
            seg.assign(l, r, {mint(b), mint(c)});
        }
        if (t == 1) {
            INT(l, r, x);
            AFF f = seg.prod(l, r);
            mint ans = Mono::eval(f, x);
            print(ans);
        }
    }
}

signed main() {
    solve();
    return 0;
}
