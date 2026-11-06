#define PROBLEM "https://judge.yosupo.jp/problem/point_add_rectangle_sum"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"
#include "library/monoid/monoid_add.hpp"

void solve() {
    LL(N, Q);
    vector<int> X(N), Y(N);
    vector<long long> W(N);
    For(i, N) read(X[i], Y[i], W[i]);
    // a point added later starts with weight 0 and gets its weight at its query
    vector<tuple<int, int, int, int>> query(Q);
    For(q, Q) {
        LL(t);
        if (t == 0) {
            INT(x, y, w);
            X.emplace_back(x), Y.emplace_back(y), W.emplace_back(0);
            query[q] = {-1, x, y, w};
        } else {
            INT(a, b, c, d);
            query[q] = {a, c, b, d};
        }
    }
    Wavelet_Matrix_2D_Range<int, true, true, FenwickTree<Monoid_Add<long long>>> WM(
        len(X), [&](int i) -> tuple<int, int, long long> { return {X[i], Y[i], W[i]}; });
    int idx = N;
    For(q, Q) {
        auto [a, b, c, d] = query[q];
        if (a == -1) {
            WM.add(idx++, d);
        } else {
            print(WM.prod(a, b, c, d));
        }
    }
}

signed main() {
    solve();
    return 0;
}
