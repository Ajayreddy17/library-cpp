#ifndef AJAY_MAXIMUM_INDEPENDENT_SET_WEIGHTED
#define AJAY_MAXIMUM_INDEPENDENT_SET_WEIGHTED

#include "library/graph/base.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/maximum_independent_set_weighted.hpp

// meet in the middle, O(N2^{N/2})
// self loops are ok (such vertices cannot be in the set)
template <typename T, typename GT>
vector<int> maximum_independent_set_weighted(GT& G, vector<T> weight) {
    const int N = G.N;
    assert(G.N <= 64);
    vector<uint64_t> nbd(N);
    For(v, N) {
        for (auto&& e: G[v]) { nbd[v] |= uint64_t(1) << (e.to); }
    }
    int NL = cld(N, 2);
    int NR = N - NL;
    vector<uint64_t> nbd_L(1 << NL);
    vector<T> wt_L(1 << NL);
    For(i, NL) For(s, 1 << i) { nbd_L[s | 1 << i] = nbd_L[s] | nbd[i]; }
    For(i, NL) For(s, 1 << i) { wt_L[s | 1 << i] = wt_L[s] + weight[i]; }
    vector<uint64_t> nbd_R(1 << NR);
    vector<T> wt_R(1 << NR);
    For(i, NR) For(s, 1 << i) { nbd_R[s | 1 << i] = nbd_R[s] | nbd[NL + i]; }
    For(i, NR) For(s, 1 << i) { wt_R[s | 1 << i] = wt_R[s] + weight[NL + i]; }

    For(s, 1 << NL) if (nbd_L[s] & s) wt_L[s] = -1;
    For(s, 1 << NR) if (nbd_R[s] >> NL & s) wt_R[s] = -1;

    vector<T> dp = wt_R;
    For(i, NR) For(s, 1 << NR) {
        int t = s | 1 << i;
        if (s < t) chmax(dp[t], dp[s]);
    }
    reverse(dp.begin(), dp.end());
    T best = 0;
    int best_s = 0;
    For(s, 1 << NL) {
        if (chmax(best, wt_L[s] + dp[(nbd_L[s] >> NL)])) best_s = s;
    }
    int s = best_s;
    int used = nbd_L[s] >> NL;
    For(t, 1 << NR) {
        if (used & t) continue;
        if (wt_L[s] + wt_R[t] == best) {
            vector<int> res;
            For(i, NL) if (s & 1 << i) res.emplace_back(i);
            For(i, NR) if (t & 1 << i) res.emplace_back(NL + i);
            return res;
        }
    }
    assert(0);
    return {};
}
} // namespace mitsuha
#endif // AJAY_MAXIMUM_INDEPENDENT_SET_WEIGHTED
