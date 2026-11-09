#ifndef AJAY_TREE_ALL_DISTANCES
#define AJAY_TREE_ALL_DISTANCES

#include "library/graph/tree/centroid_decomposition.hpp"
#include "library/poly/convolution.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_all_distances.hpp
// ANS[d] = #{unordered pairs {u, v} : dist(u, v) = d}, ANS[0] = 0
// sum of result array = binom(N, 2)
template <typename GT>
vector<long long> tree_all_distances(GT& G) {
    assert(G.is_prepared());
    int N = G.N;
    vector<long long> ANS(N);
    auto f = [&](vector<int>& par, vector<int>& V, int L1, int R1, int L2, int R2) -> void {
        int N = len(par);
        vector<int> dist(N);
        For(i, 1, N) { dist[i] = 1 + dist[par[i]]; }
        int mx = *max_element(dist.begin(), dist.end());
        vector<long long> f(1 + mx), g(1 + mx);
        For(i, L1, R1) f[dist[i]]++;
        For(i, L2, R2) g[dist[i]]++;
        while (len(f) && f.back() == 0) f.pop_back();
        while (len(g) && g.back() == 0) g.pop_back();
        f = convolution(f, g);
        For(i, len(f)) ANS[i] += f[i];
    };
    centroid_decomposition<1>(G, f);
    if (N >= 2) ANS[1] = N - 1;
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_TREE_ALL_DISTANCES