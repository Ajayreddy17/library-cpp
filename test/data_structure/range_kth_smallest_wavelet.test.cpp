#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"
#include <library/template.hpp>
#include <library/datastructure/wavelet_matrix/wavelet_matrix.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    Wavelet_Matrix<int, false> WM(A);
    For(Q) {
        LL(l, r, k);
        print(WM.kth(l, r, k));
    }
}

signed main() {
    solve();

    return 0;
}
