#define PROBLEM "https://yukicoder.me/problems/no/1919"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/monoid/monoid_add_pair.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

using mint = modint107;

void solve() {
    LL(N);
    VT(int, A, N);
    VT(int, B, N);
    pair<mint, mint> ANS;
    For(2) {
        swap(ANS.first, ANS.second);
        swap(A, B);
        auto I = argsort(A);
        A = rearrange(A, I);
        B = rearrange(B, I);
        vector<int> X(N), Y(N);
        For(i, N) X[i] = A[i] - B[i];
        For(i, N) Y[i] = A[i] + B[i];
        using Grp = Monoid_Add_Pair<mint>;
        Wavelet_Matrix_2D_Range<int, true, true, FenwickTree<Grp>> WM(
            N, [&](int i) -> tuple<int, int, pair<mint, mint>> { return {X[i], Y[i], Grp::unit()}; });
        For(i, N) {
            WM.multiply(i, {mint(1), mint(A[i])});
            auto [c, s] = WM.prod(-iinf, X[i], -iinf, Y[i]);
            ANS.first += mint(A[i]) * c - s;
        }
    }
    ANS.first *= mint(2);
    ANS.second *= mint(2);
    print(ANS.first, ANS.second);
}

signed main() {
    solve();
    return 0;
}
