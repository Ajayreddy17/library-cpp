#define PROBLEM "https://judge.yosupo.jp/problem/rectangle_sum"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/static_range_product_group.hpp"
#include "library/monoid/monoid_add.hpp"

void solve() {
    LL(N, Q);
    vector<int> X(N), Y(N), W(N);
    For(i, N) read(X[i], Y[i], W[i]);
    Wavelet_Matrix_2D_Range<int, true, true, Static_Range_Product_Group<Monoid_Add<long long>>> WM(
        N, [&](int i) -> tuple<int, int, long long> { return {X[i], Y[i], W[i]}; });
    For(Q) {
        LL(a, c, b, d);
        print(WM.prod(a, b, c, d));
    }
}

signed main() {
    solve();
    return 0;
}
