#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/random/base.hpp>
#include <library/datastructure/fenwick_tree/dual_fenwicktree.hpp>

void test() {
    For(N, 1, 50) {
        For(Q, 1, 100) {
            vector<int> A(N);
            Dual_FenwickTree<Monoid_Add<int>> bit(N);
            For(Q) {
                int t = RNG(0, 3);
                if (t == 0) {
                    // apply
                    int L = RNG(0, N), R = RNG(0, N);
                    if (L > R) swap(L, R);
                    int x = RNG(0, 100);
                    ++R;
                    For(i, L, R) A[i] += x;
                    bit.apply(L, R, x);
                }
                if (t == 1) {
                    // get
                    int i = RNG(0, N);
                    assert(A[i] == bit.get(i));
                }
                if (t == 2) {
                    // get_all
                    assert(A == bit.get_all());
                }
            }
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}

signed main() {
    test();
    solve();
    return 0;
}