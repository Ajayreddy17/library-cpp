#define PROBLEM "https://yukicoder.me/problems/no/2859"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix_2d_range.hpp"
#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_max.hpp"

void solve() {
    LL(N, K);
    VT(long long, T, N);
    VT(long long, X, N);
    VT(long long, C, N);
    auto I = argsort(T);
    T = rearrange(T, I);
    X = rearrange(X, I);
    C = rearrange(C, I);
    vector<long long> A(N), B(N);
    For(i, N) A[i] = X[i] + K * T[i];
    For(i, N) B[i] = K * T[i] - X[i];
    Wavelet_Matrix_2D_Range<long long, 1, 1, SegTree<Monoid_Max<long long>>> seg(
        N, [&](int i) -> tuple<long long, long long, long long> { return {A[i], B[i], -linf}; });
    long long ANS = 0;
    For(i, N) {
        if (abs(X[i]) > K * T[i]) continue;
        long long ans = 0;
        chmax(ans, seg.prefix_prod(-linf, A[i] + 1, B[i] + 1));
        ans += C[i];
        seg.set(i, ans);
        chmax(ANS, ans);
    }
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
