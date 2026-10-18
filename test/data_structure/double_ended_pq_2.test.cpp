#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"

#include <library/template.hpp>
#include <library/datastructure/segment_tree/dynamic_segtree_sparse.hpp>
#include <library/monoid/monoid_add.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    const int LIM = 1'000'000'000;

    Dynamic_SegTree_Sparse<Monoid_Add<int>, false> seg(-LIM, LIM + 1);
    int root = decltype(seg)::NIL;
    for (auto&& a : A) root = seg.multiply(root, a, 1);

    For(Q) {
        LL(t);
        if (t == 0) {
            LL(x);
            root = seg.multiply(root, x, 1);
        }
        if (t == 1) {
            auto check = [&](auto e) -> bool { return e == 0; };
            int ANS = seg.max_right(root, check, -LIM);
            print(ANS);
            root = seg.multiply(root, ANS, -1);
        }
        if (t == 2) {
            auto check = [&](auto e) -> bool { return e == 0; };
            int ANS = seg.min_left(root, check, LIM + 1) - 1;
            print(ANS);
            root = seg.multiply(root, ANS, -1);
        }
    }
}

signed main() {
    solve();

    return 0;
}
