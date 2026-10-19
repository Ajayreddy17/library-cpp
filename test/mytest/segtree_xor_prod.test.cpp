#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/monoid/monoid_add.hpp>
#include <library/random/base.hpp>
#include <library/datastructure/segment_tree/segtree.hpp>

void test() {
    For(100) {
        int n = RNG(0, 5);
        int N = 1 << n;
        using Mono = Monoid_Add<int>;
        vector<int> A(N);
        For(i, N) A[i] = RNG(0, 10);
        SegTree<Mono> seg(A);
        For(100) {
            int t = RNG(0, 2);
            if (t == 0) {
                int i = RNG(0, N);
                int x = RNG(0, 10);
                seg.set(i, x);
                A[i] = x;
            }
            if (t == 1) {
                int L = RNG(0, N);
                int R = RNG(0, N);
                if (L > R) swap(L, R);
                ++R;
                int xor_val = RNG(0, N);
                int sm = 0;
                For(i, L, R) sm += A[i ^ xor_val];
                assert(sm == seg.xor_prod(L, R, xor_val));
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    solve();
    return 0;
}
