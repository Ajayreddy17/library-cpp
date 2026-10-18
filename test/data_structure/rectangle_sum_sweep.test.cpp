#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree.hpp>
#include <library/datastructure/offline_query/point_add_rectangle_sum.hpp>

void solve() {
    LL(N, Q);
    Point_Add_Rectangle_Sum<Monoid_Add<long long>, int> RS;
    For(N) {
        LL(x, y, w);
        RS.add_query(x, y, w);
    }
    For(Q) {
        LL(l, d, r, u);
        RS.sum_query(l, r, d, u);
    }
    auto ANS = RS.calc();
    for (auto&& x: ANS) print(x);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
