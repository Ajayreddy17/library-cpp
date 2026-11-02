#define PROBLEM "https://yukicoder.me/problems/no/1526"
#include "library/template.hpp"
#include "library/datastructure/segment_tree/beats_summax_chmin.hpp"

// test 用に、逆符号で実装
void solve() {
    LL(N);
    vector<long long> A(N); read(A);
    for (auto&& x: A) --x;

    Beats_SumMax_Chmin<long long> X(N, [](int i) -> long long { return -(i + 1); });

    vector<vector<int>> IDS(N + 1);
    For(i, N) IDS[A[i]].emplace_back(i);

    long long ANS = 0;
    For(x, N + 1) {
        // ANS += (N + 1) * N - accumulate(dp.begin(), dp.end(), (int)0);
        ANS += (N + 1) * N + X.prod(0, N).first;
        auto& I = IDS[x];
        I.insert(I.begin(), -1);
        I.emplace_back(N);
        For(t, len(I) - 1) {
            auto i = I[t], j = I[t + 1];
            // For(k, i + 1, j) chmax(dp[k], j + 1);
            X.chmin(i + 1, j, -(j + 1));
        }
    }
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
