#define PROBLEM "https://yukicoder.me/problems/no/1234"
#include "library/template.hpp"
#include "library/acted_monoid/am_min_add.hpp"
#include "library/datastructure/segment_tree/lazy_segtree.hpp"

void solve() {
    LL(N);
    vector<long long> A(N); read(A);
    Lazy_SegTree<ActedMonoid_Min_Add<long long>> seg(A);
    LL(Q);
    For(Q) {
        LL(t, l, r, x);
        --l;
        if (t == 1) { seg.apply(l, r, x); }
        if (t == 2) print(seg.prod(l, r));
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
