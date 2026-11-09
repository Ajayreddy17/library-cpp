#define PROBLEM "https://yukicoder.me/problems/no/2892"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/tree/centroid_decomposition.hpp"

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    G.read_tree();
    STR(S);

    long long ANS = 0;

    auto f = [&](vector<int>& par, vector<int>& V, int L1, int R1, int L2, int R2) -> void {
        int n = len(V);
        vector<int> A(n);
        For(i, n) { A[i] = (S[V[i]] == '1' ? 1 : -1); }
        // value of the path from i up to (excluding) the root V[0]
        vector<int> dp(n);
        For(i, 1, n) dp[i] = dp[par[i]] + A[i];
        // F[k] = #{j in [L2, R2) : n - dp[j] < k}
        vector<int> F(2 * n + 4);
        For(j, L2, R2) F[n - dp[j] + 1]++;
        For(k, len(F) - 1) F[k + 1] += F[k];
        For(i, L1, R1) ANS += F[dp[i] + A[0] + n];
    };

    centroid_decomposition<1>(G, f);
    For(v, N) if (S[v] == '1') ++ANS;
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
