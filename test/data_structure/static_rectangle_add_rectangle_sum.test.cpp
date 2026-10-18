#define PROBLEM \
    "https://judge.yosupo.jp/problem/static_rectangle_add_rectangle_sum"
#include <library/template.hpp>

#include <library/datastructure/offline_query/rectangle_add_rectangle_sum.hpp>
#include <library/mod/modint.hpp>
#include <library/monoid/monoid_add.hpp>

using mint = modint998;

void solve() {
    LL(N, Q);
    Rectangle_Add_Rectangle_Sum<mint> X;

    For(N) {
        LL(l, d, r, u, w);
        X.add_query(l, r, d, u, w);
    }
    For(Q) {
        LL(l, d, r, u);
        X.sum_query(l, r, d, u);
    }
    print(X.calc());
}

signed main() {
    solve();
    return 0;
}
