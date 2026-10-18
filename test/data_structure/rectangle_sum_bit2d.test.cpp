#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree_2d.hpp>

void solve() {
    LL(N, Q);
    vector<long long> X(N), Y(N), W(N);
    For(i, N) {
        LL(x, y, w);
        X[i] = x, Y[i] = y, W[i] = w;
    }
    FenwickTree_2D<Monoid_Add<long long>, long long, false> bit(X, Y, W);
    For(Q) {
        LL(l, d, r, u);
        print(bit.sum(l, r, d, u));
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
