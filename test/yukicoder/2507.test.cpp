#define PROBLEM "https://yukicoder.me/problems/no/2507"
#include "library/template.hpp"
#include "library/setfunc/subset_convolution.hpp"
#include "library/setfunc/sps_exp.hpp"
#include "library/graph/count/count_cycle.hpp"

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M);

    vector<uint64_t> dp = count_cycle<uint64_t>(G);
    For(v, N) dp[1 << v] += 1;
    for (auto& e: G.edges) {
        int a = e.frm, b = e.to;
        vector<uint64_t> A(1 << N), B(1 << N);
        For(s, 1 << N) {
            if (s >> a & 1) A[s] += dp[s];
            if (s >> b & 1) B[s] += dp[s];
        }
        A = subset_convolution<uint64_t, 13>(A, B);
        For(s, 1 << N) { dp[s] += A[s]; }
    }

    dp = sps_exp<uint64_t, 13>(dp);
    print(dp.back());
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
