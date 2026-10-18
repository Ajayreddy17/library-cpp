#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_point_get"
#include <library/template.hpp>

#include <library/datastructure/segment_tree/dual_segtree.hpp>
#include <library/monoid/monoid_affine.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;

void solve() {
    INT(N, Q);
    vector<mint> A(N);
    For(i, N) {
        uint32_t x;
        read(x);
        A[i] = mint::raw(x);
    }
    Dual_SegTree<Monoid_Affine<mint>> seg(N);
    For(Q) {
        LL(t);
        if (t == 0) {
            uint32_t l, r, b, c;
            read(l, r, b, c);
            seg.apply(l, r, {mint::raw(b), mint::raw(c)});
        }
        if (t == 1) {
            uint32_t idx;
            read(idx);
            mint x = A[idx];
            auto [a, b] = seg.get(idx);
            print(a * x + b);
        }
    }
}

signed main() {
    solve();
    return 0;
}
