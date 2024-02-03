#ifndef AJAY_EDIT_DISTANCE
#define AJAY_EDIT_DISTANCE

namespace mitsuha{
// Insertion Deletion Substitution 
template <typename STRING>
int edit_distance(STRING& S, STRING& T) {
    int N = len(S), M = len(T);
    vector<int> dp(M + 1, 1 << 30);
    dp[0] = 0;
    For(i, N + 1) {
        vector<int> newdp(M + 1, 1 << 30);
        For(j, M + 1) {
            if (i < N) chmin(newdp[j], dp[j] + 1);
            if (j < M) chmin(dp[j + 1], dp[j] + 1);
            if (i < N && j < M) chmin(newdp[j + 1], dp[j] + 1);
            if (i < N && j < M && S[i] == T[j]) chmin(newdp[j + 1], dp[j]);
        }
        if (i < N) swap(dp, newdp);
    }
    return dp[M];
}
} // namespace mitsuha
#endif // AJAY_EDIT_DISTANCE