#ifndef AJAY_INVERSIONS
#define AJAY_INVERSIONS

#include "library/datastructure/fenwick_tree/fenwicktree_01.hpp"

namespace mitsuha {
template <typename T>
long long inversion(vector<T> A) {
    int N = len(A);
    if (A.empty()) return 0;
    long long ANS = 0;
    FenwickTree_01 bit(N);
    auto I = argsort(A);
    for (auto& i : I) {
        ANS += bit.sum_all() - bit.sum(i);
        bit.add(i, 1);
    }
    return ANS;
}

// inversions when rotated so that A_i is at the front
template <typename T, bool SMALL = false>
vector<long long> inversion_rotate(vector<T>& A) {
    const int N = len(A);
    if (not SMALL) {
        auto key = A;
        settify(key);
        for (auto&& x : A) x = lower_bound(key.begin(), key.end(), x) - key.begin();
    }
    long long K = *max_element(A.begin(), A.end()) + 1;
    long long ANS = 0;
    FenwickTree<Monoid_Add<int>> bit(K);
    for (auto&& x : A) {
        ANS += bit.sum(x + 1, K);
        bit.add(x, 1);
    }
    vector<long long> res(N);
    For(i, N) {
        res[i] = ANS;
        long long x = A[i];
        ANS = ANS + bit.sum(x + 1, K) - bit.prefix_sum(x);
    }
    return res;
}

// inv[i][j] = inversions in A[i:j)
template <typename T>
vector<vector<long long>> all_range_inversion(vector<T>& A) {
    int N = len(A);
    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1));
    Frr(L, N + 1) For(R, L + 2, N + 1) {
        dp[L][R] = dp[L][R - 1] + dp[L + 1][R] - dp[L + 1][R - 1];
        if (A[L] > A[R - 1]) ++dp[L][R];
    }
    return dp;
}
} // namespace mitsuha
#endif // AJAY_INVERSIONS