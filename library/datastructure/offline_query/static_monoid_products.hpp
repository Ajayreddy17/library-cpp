#ifndef AJAY_STATIC_MONOID_PRODUCTS
#define AJAY_STATIC_MONOID_PRODUCTS

namespace mitsuha{
template <typename Mono, typename T>
vector<T> static_monoid_products(vector<T>& A, vector<pair<int, int>>& query) {
    int N = len(A), Q = len(query);

    vector<T> ANS(Q);
    vector<vector<int>> IDS(N);
    For(q, Q) {
        auto [L, R] = query[q];
        if (L == R) { ANS[q] = Mono::unit(); }
        elif (R <= L + 32) {
            ANS[q] = A[L];
            For(i, L + 1, R) ANS[q] = Mono::op(ANS[q], A[i]);
        }
        else {
            --R;
            int k = ((L ^ R) == 0 ? -1 : 31 - __builtin_clz(L ^ R));
            int M = R >> k << k;
            IDS[M].emplace_back(q);
        }
    }
    vector<T> dp(N + 1);

    For(M, N) {
        auto& I = IDS[M];
        if (I.empty()) continue;
        int min_a = M, max_b = M;
        for (int q: I) {
            auto [a, b] = query[q];
            min_a = min(min_a, a), max_b = max(max_b, b);
        }
        // Calculation of cumulative product
        dp[M] = Mono::unit();
        for (int i = M; i > min_a; --i) dp[i - 1] = Mono::op(A[i - 1], dp[i]);
        for (int i = M; i < max_b; ++i) dp[i + 1] = Mono::op(dp[i], A[i]);
        // calculating the answer
        for (int q: I) {
            auto [a, b] = query[q];
            ANS[q] = Mono::op(dp[a], dp[b]);
        }
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_STATIC_MONOID_PRODUCTS