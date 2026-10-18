#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"

#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree.hpp>
#include <library/datastructure/offline_query/coefficient_query_2d.hpp>

void solve() {
    LL(N, Q);
    Coefficient_Query_2D<1, 1, long long, true> X;
    For(i, N) {
        LL(x, y, w);
        X.add_query(x, y, w);
    }
    For(Q) {
        LL(x1, y1, x2, y2);
        --x1, --y1, --x2, --y2;
        X.sum_query(x1, y1);
        X.sum_query(x1, y2);
        X.sum_query(x2, y1);
        X.sum_query(x2, y2);
    }
    auto res = X.calc();
    vector<long long> ANS(Q);
    For(q, Q)
    ANS[q] = res[4 * q] - res[4 * q + 1] - res[4 * q + 2] + res[4 * q + 3];
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
