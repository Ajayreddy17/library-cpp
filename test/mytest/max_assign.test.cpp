#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/acted_monoid/am_max_assign.hpp>
#include <library/random/base.hpp>
#include <library/datastructure/segment_tree/lazy_segtree.hpp>

void test() {
    int N = RNG(1, 100);
    vector<int> A(N);
    For(i, N) A[i] = RNG(1, 100);
    Lazy_SegTree<ActedMonoid_Max_Assign<int, -1>> seg(A);
    int Q = RNG(1, 100);
    For(Q) {
        long long t = RNG(0, 2);
        long long L = RNG(0, N);
        long long R = RNG(0, N);
        if (L > R) swap(L, R);
        ++R;
        if (t == 1) {
            long long x = RNG(1, 100);
            For(i, L, R) A[i] = x;
            seg.apply(L, R, x);
        }
        if (t == 2) {
            vector<int> B = {A.begin() + L, A.begin() + R};
            assert(seg.prod(L, R) == *max_element(B.begin(), B.end()));
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);
    For(100) test();
    solve();

    return 0;
}
