#define PROBLEM "https://yukicoder.me/problems/no/919"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"

void solve() {
    LL(N);
    VT(long long, A, N);
    Wavelet_Matrix<long long, true> WM(A);
    long long ANS = -linf;
    auto get = [&](long long L, long long R) -> long long { return WM.median<false>(L, R); };
    For(K, 1, N + 1) {
        // at most LIM blocks of length K from each side
        long long LIM = N / K;
        vector<long long> left(LIM + 1), right(LIM + 1);
        For(i, LIM) left[i + 1] = left[i] + get(K * i, K * i + K);
        For(i, LIM) right[i + 1] = right[i] + get(N - K * i - K, N - K * i);
        For(i, LIM) chmax(left[i + 1], left[i]);
        For(i, LIM) chmax(right[i + 1], right[i]);
        For(i, LIM + 1) chmax(ANS, K * (left[i] + right[LIM - i]));
    }
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
