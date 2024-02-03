#ifndef AJAY_LONGEST_COMMON_SUBSEQ
#define AJAY_LONGEST_COMMON_SUBSEQ

namespace mitsuha{
template <typename STRING>
int longest_common_subsequence(STRING& A, STRING& B) {
    int N = len(B);
    vector<int> dp(N + 1);
    for (auto&& a: A) {
        Frr(i, N) if (a == B[i]) chmax(dp[i + 1], dp[i] + 1);
        For(i, N) chmax(dp[i + 1], dp[i]);
    }
    return dp[N];
}

/*
LCS dp that also restores.
Returns a vector of (i, j) pairs using (A[i], B[j]).
*/
template <typename STRING>
vector<pair<int, int>> longest_common_subsequence_restore(STRING& A, STRING& B) {
    int N = len(A), M = len(B);
    vector<vector<int>> DP(N + 1, vector<int>(M + 1));
    For(i, N) {
        auto& dp = DP[i];
        auto& newdp = DP[i + 1];
        newdp = dp;
        For(j, M) {
            chmax(newdp[j + 1], newdp[j]);
            if (A[i] == B[j]) chmax(newdp[j + 1], dp[j] + 1);
        }
    }
    vector<pair<int, int>> res;
    int n = N, m = M;
    while (DP[n][m]) {
        if (DP[n][m] == DP[n - 1][m]) { --n; }
        elif (DP[n][m] == DP[n][m - 1]) { --m; }
        else {
            --n, --m;
            res.emplace_back(n, m);
        }
    }
    reverse(res.begin(), res.end());
    return res;
}
} // namespace mitsuha
#endif // AJAY_LONGEST_COMMON_SUBSEQ