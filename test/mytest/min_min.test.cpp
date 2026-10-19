#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/io/io.hpp>
#include <library/acted_monoid/am_min_min.hpp>
#include <library/random/base.hpp>
#include <library/datastructure/segment_tree/lazy_segtree.hpp>

void test() {
    int N = RNG(1, 100);
    vector<int> A(N);
    For(i, N) A[i] = RNG(1, 100);
    using AM = ActedMonoid_Min_Min<int>;
    Lazy_SegTree<AM> seg(A);
    int Q = RNG(1, 100);
    For(Q) {
        long long t = RNG(0, 2);
        long long L = RNG(0, N);
        long long R = RNG(0, N);
        if (L > R) swap(L, R);
        ++R;
        if (t == 1) {
            long long x = RNG(1, 100);
            For(i, L, R) chmin(A[i], x);
            seg.apply(L, R, x);
        }
        if (t == 2) {
            vector<int> B = {A.begin() + L, A.begin() + R};
            assert(seg.prod(L, R) == *min_element(B.begin(), B.end()));
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);
    For(100) test();
    solve();

    return 0;
}
