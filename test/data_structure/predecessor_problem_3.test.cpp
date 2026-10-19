#define PROBLEM "https://judge.yosupo.jp/problem/predecessor_problem"
#include <library/template.hpp>

#include <library/datastructure/segment_tree/segtree.hpp>
#include <library/monoid/monoid_add.hpp>

void solve() {
    INT(N, Q);
    STR(T);

    SegTree<Monoid_Add<int>> seg(N, [&](int i) -> int { return T[i] - '0'; });

    For(Q) {
        INT(t, k);
        if (t == 0) {
            if (seg.prod(k, k + 1) == 0) seg.set(k, 1);
        }
        if (t == 1) {
            if (seg.prod(k, k + 1)) seg.set(k, 0);
        }
        if (t == 2) { print(seg.get(k)); }
        if (t == 3) {
            int ans = seg.max_right(k, [&](auto e) -> bool { return e == 0; });
            if (ans == N) ans = -1;
            print(ans);
        }
        if (t == 4) {
            int ans = seg.min_left(k + 1, [&](auto e) -> bool { return e == 0; }) - 1;
            print(ans);
        }
    }
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
