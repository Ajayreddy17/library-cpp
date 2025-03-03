#ifndef AJAY_LONGEST_INCRESING_SUQSEQUENCE
#define AJAY_LONGEST_INCRESING_SUQSEQUENCE

namespace mitsuha{
// dp[i] := maximum length of LIS ending with the i-th term ([1, LIS])
template <typename T, bool strict = true>
pair<int, vector<int>> longest_increasing_subsequence_dp(vector<T> A) {
    const int N = A.size();
    vector<T> dp(N, numeric_limits<T>::max() / 2);
    int lis = 0;
    vector<int> lis_rank(N);
    For(i, N) {
        int j = (strict ? lower_bound(dp.begin(), dp.end(), A[i]) - dp.begin() 
                        : upper_bound(dp.begin(), dp.end(), A[i]) - dp.begin());
        dp[j] = A[i];
        lis_rank[i] = j + 1;
        chmax(lis, j + 1);
    }
    return {lis, lis_rank};
}

template <typename T, bool strict = true>
vector<int> longest_increasing_subsequence(vector<T> A) {
    const int N = A.size();
    auto [lis, dp] = longest_increasing_subsequence_dp<T, strict>(A);
    vector<int> I;
    For(i, N) if (dp[i] == lis) {
        I.emplace_back(i);
        break;
    }
    For(lis - 1) {
        int i = I.back();
        Frr(j, i) {
            bool ok = (dp[j] == dp[i] - 1) && (strict ? (A[j] < A[i]) : (A[j] <= A[i]));
            if (ok) {
                I.emplace_back(j);
                break;
            }
        }
    }
    assert(len(I) == lis);
    reverse(I.begin(), I.end());
    return I;
}
} // namespace mitsuha
#endif // AJAY_LONGEST_INCRESING_SUQSEQUENCE