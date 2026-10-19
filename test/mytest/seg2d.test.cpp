#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/random/base.hpp>
#include <library/datastructure/segment_tree/segtree_2d.hpp>
#include <library/monoid/monoid_min.hpp>

void test() {
    For(N, 100) {
        For(Q, 100) {
            vector<int> X(N), Y(N), val(N);
            For(i, N) X[i] = RNG(0, 5), Y[i] = RNG(0, 5), val[i] = RNG(0, 100);
            SegTree_2D<Monoid_Min<int>, int, true> seg(X, Y, val);
            For(Q) {
                int t = RNG(0, 3);
                if (N == 0) t = 2;
                if (t == 0) {
                    int i = RNG(0, N);
                    val[i] = RNG(0, 100);
                    seg.set(i, val[i]);
                }
                if (t == 1) {
                    int i = RNG(0, N);
                    int x = RNG(0, 100);
                    chmin(val[i], x);
                    seg.multiply(i, val[i]);
                }
                if (t == 2) {
                    int a = RNG(0, 5), b = RNG(0, 5), c = RNG(0, 5), d = RNG(0, 5);
                    if (a > b) swap(a, b);
                    if (c > d) swap(c, d);
                    int ans = iinf;
                    For(i, N) {
                        if (a <= X[i] && X[i] < b && c <= Y[i] && Y[i] < d)
                            chmin(ans, val[i]);
                    }
                    assert(ans == seg.prod(a, b, c, d));
                }
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
    test();
    solve();
    return 0;
}
