#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include <library/template.hpp>
#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_min.hpp>

void solve() {
    LL(N, Q);
    using Mono = Monoid_Min<int>;
    SegTree<Mono> seg(N, [&](int i) -> int {
        INT(x);
        return x;
    });
    For(Q) {
        LL(L, R);
        print(seg.prod(L, R));
    }
}

signed main() {
    solve();

    return 0;
}
