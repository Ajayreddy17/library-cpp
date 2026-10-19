#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/acted_monoid/am_minidx_add.hpp>
#include <library/random/base.hpp>
#include <library/datastructure/segment_tree/lazy_segtree.hpp>

void test() {
    int N = RNG(1, 100);
    vector<int> A(N);
    For(i, N) A[i] = RNG(1, 100);
    using AM = ActedMonoid_MinIdx_Add<int>;
    using Mono = typename AM::Monoid_X;
    Lazy_SegTree<AM> seg(N, [&](int i) -> typename Mono::value_type {
        return {A[i], i};
    });
    int Q = RNG(1, 100);
    For(Q) {
        long long t = RNG(0, 2);
        long long L = RNG(0, N);
        long long R = RNG(0, N);
        if (L > R) swap(L, R);
        ++R;
        if (t == 1) {
            long long x = RNG(1, 100);
            For(i, L, R) A[i] += x;
            seg.apply(L, R, x);
        }
        if (t == 2) {
            vector<int> B = {A.begin() + L, A.begin() + R};
            int mx = *min_element(B.begin(), B.end());
            int idx = -1;
            Frr(i, L, R) if (A[i] == mx) idx = i;
            assert(seg.prod(L, R) == mp(mx, idx));
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    For(100) test();
    solve();

    return 0;
}