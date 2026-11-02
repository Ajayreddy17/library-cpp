#define PROBLEM "https://yukicoder.me/problems/no/1685"
#include "library/template.hpp"
#include "library/poly/convolution.hpp"

using mint = modint107;

void solve() {
    LL(N, M);

    vector<mint> dp(M + 1);
    For(i, M + 1) if (i % 2 == 0) dp[i] = 1;
    vector<mint> comb = {1};

    int LIM_a = N / 2;
    int LIM_b = (N - 1) / 2;
    mint ANS = 0;
    For(s, min(N, M) + 1) {
        int lo = max<int>(0, s - LIM_b);
        int hi = min<int>(LIM_a, s);
        mint ans = 0;
        For(a, lo, hi + 1) {
            int b = s - a;
            int c = N - s;
            int shift = c - abs(a - b);
            mint cf = comb[a];
            mint x = dp[M - s];
            if ((N & 1) && (s + shift <= M)) x += dp[M - s - shift];
            ans += cf * x;
        }
        ANS += ans * fact_invs<mint>(s, N - s);
        For(j, M - s) dp[j + 1] += dp[j];
        comb.emplace_back(0);
        Frr(j, len(comb) - 1) comb[j + 1] += comb[j];
    }
    ANS *= fact<mint>(N);
    print(ANS);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
