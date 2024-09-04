#ifndef AJAY_TREE_ALL_DISTANCES
#define AJAY_TREE_ALL_DISTANCES

#include "library/graph/centroid_decomposition.hpp"
#include "library/poly/convolution.hpp"

namespace mitsuha{
// frequency table of distance of all directed pairs.
// sum of result array = N^2
template <typename GT>
vector<long long> tree_all_distances(GT& G) {
    assert(G.is_prepared());
    int N = G.N;
    vector<long long> ANS(N);
    auto f = [&](vector<int>& par, vector<int>& V, int n1, int n2) -> void {
        int N = len(par);
        vector<int> dist(N);
        For(i, 1, N) { dist[i] = 1 + dist[par[i]]; }
        int mx = *max_element(dist.begin(), dist.end());
        vector<long long> f(1 + mx), g(1 + mx);
        For(i, 1, 1 + n1) f[dist[i]]++;
        For(i, 1 + n1, 1 + n1 + n2) g[dist[i]]++;
        while (len(f) && f.back() == 0) f.pop_back();
        while (len(g) && g.back() == 0) g.pop_back();
        f = convolution(f, g);
        For(i, len(f)) ANS[i] += f[i] * 2;
    };
    centroid_decomposition<1>(G, f);
    ANS[0] = N, ANS[1] = 2 * (N - 1);
    return ANS;
}
} // namespace mitsuha
#endif // AJAY_TREE_ALL_DISTANCES