#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_add_point_get"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/dual_fenwicktree_2d.hpp>

void solve() {
    LL(N, Q);
    using T = tuple<int, int, int, int, int>;
    VT(T, dat, N);
    vector<T> QUERY;
    vector<int> X, Y;
    For(Q) {
        INT(t);
        if (t == 0) {
            LL(a, b, c, d, x);
            QUERY.emplace_back(a, b, c, d, x);
        }
        if (t == 1) {
            LL(x, y);
            X.emplace_back(x), Y.emplace_back(y);
            QUERY.emplace_back(x, y, -1, -1, -1);
        }
    }

    Dual_FenwickTree_2D<Monoid_Add<long long>, int, false> bit(X, Y);

    for (auto& [a, b, c, d, x]: dat) bit.apply(a, c, b, d, x);
    for (auto& [a, b, c, d, x]: QUERY) {
        if (x == -1) {
            long long ans = bit.get(a, b);
            print(ans);
        } else {
            bit.apply(a, c, b, d, x);
        }
    }
}

signed main() {
    solve();
    return 0;
}
