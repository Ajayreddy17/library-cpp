#ifndef AJAY_CHROMATIC_NUMBER
#define AJAY_CHROMATIC_NUMBER

#include "library/graph/base.hpp"
#include "library/random/base.hpp"
#include "library/number/primetest.hpp"

namespace mitsuha{
// O(N2^N)
template <typename Graph, int TRIAL = 0>
int chromatic_number(Graph& G) {
    assert(G.is_prepared());

    int N = G.N;
    vector<int> nbd(N);
    For(v, N) for (auto&& e: G[v]) nbd[v] |= 1 << e.to;
    
    // Enumeration of independent sets such that they are subsets of s
    vector<int> dp(1 << N);
    dp[0] = 1;
    For(v, N) For(s, 1 << v) { dp[s | 1 << v] = dp[s] + dp[s & (~nbd[v])]; }

    vector<long long> pow(1 << N);
    auto solve_p = [&](int p) -> int {
        For(s, 1 << N) pow[s] = ((N - __builtin_popcount(s)) & 1 ? 1 : -1);
        For(k, 1, N) {
            long long sum = 0;
            For(s, 1 << N) {
                pow[s] = pow[s] * dp[s];
                if (p) pow[s] %= p;
                sum += pow[s];
            }
            if (p) sum %= p;
            if (sum != 0) { return k; }
        }
        return N;
    };

    int ANS = 0;
    chmax(ANS, solve_p(0));

    Loop(TRIAL) {
        int p;
        while (1) {
            p = RNG(1LL << 30, 1LL << 31);
            if (primetest(p)) break;
        }
        chmax(ANS, solve_p(p));
    }
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_CHROMATIC_NUMBER