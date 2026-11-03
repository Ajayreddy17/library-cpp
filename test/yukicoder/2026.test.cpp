#define PROBLEM "https://yukicoder.me/problems/no/2026"
#include "library/template.hpp"
#include "library/datastructure/sliding_window_aggregation.hpp"
#include "library/monoid/monoid_max.hpp"

using Mono = Monoid_Max<long long>;

void solve() {
    LL(N);
    vector<pair<long long, long long>> CV(N); read(CV);
    CV.insert(CV.begin(), {0, 0});

    // count, weight
    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1, -linf));
    dp[0][0] = 0;

    Frr(wt, 1, N + 1) {
        auto [cnt, val] = CV[wt];
        int LIM = N / wt;
        For(i, LIM + 1) For(j, N + 1) {
            if (i >= 1 && j >= wt) continue;
            int n = min(LIM - i, (N - j) / wt);
            vector<long long> A(n + 1);
            For(k, n + 1) A[k] = dp[i + k][j + wt * k];

            Sliding_Window_Aggregation<Mono> X;
            For(k, n + 1) {
                X.push(A[k] - val * k);
                if (len(X) > cnt + 1) X.pop();
                A[k] = X.prod() + val * k;
            }
            For(k, n + 1) dp[i + k][j + wt * k] = A[k];
        }
    }
    For(k, 1, N + 1) print(*max_element(dp[k].begin(), dp[k].end()));
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
