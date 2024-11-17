#ifndef AJAY_DIGIT_SUM
#define AJAY_DIGIT_SUM

namespace mitsuha{
int digit_sum(unsigned long long x) {
    const int K = 100'000;
    static vector<int> dp(K);
    if (dp[1] == 0) {
        For(x, 1, K) dp[x] = dp[x / 10] + (x % 10);
    }
    int res = 0;
    while (x) {
        res += dp[x % K];
        x /= K;
    }
    return res;
}
} // namespace mitsuha
#endif // AJAY_DIGIT_SUM