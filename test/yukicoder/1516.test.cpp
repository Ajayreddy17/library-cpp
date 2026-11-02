#define PROBLEM "https://yukicoder.me/problems/no/1516"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/seq/interpolate_linear_rec.hpp"

using mint = modint998;

void solve() {
    LL(N, K);
    long long LIM = 300;
    vector<mint> A(LIM);
    vector<mint> B(LIM);
    using P = pair<mint, mint>;

    vector<vector<P>> dp(K, vector<P>(K));
    For(a, K) For(b, K) if (a != b) dp[a][b] = {mint(1), mint(a + b)};

    For(k, 2, LIM) {
        For(a, K) For(b, K) {
            A[k] += dp[a][b].first;
            B[k] += dp[a][b].second;
        }
        vector<vector<P>> newdp(K, vector<P>(K));
        For(a, K) For(b, K) For(c, K) {
            if (a == b || a == c || b == c) continue;
            if (a < b && b < c) continue;
            if (a > b && b > c) continue;
            newdp[b][c].first += dp[a][b].first;
            newdp[b][c].second += dp[a][b].second + mint(c) * dp[a][b].first;
        }
        swap(dp, newdp);
    }

    mint a = interpolate_linear_rec<mint>(A, N, 10);
    mint b = interpolate_linear_rec<mint>(B, N, 10);
    print(a, b);
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
