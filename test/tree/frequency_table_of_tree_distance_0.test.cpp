#define PROBLEM \
    "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"

#include <library/template.hpp>

#include <library/graph/centroid_decomposition.hpp>
#include <library/poly/convolution.hpp>

void solve() {
    LL(N);
    Graph<int> G(N);
    G.read_tree(0, 0);
    vector<long long> ANS(N);
    auto f = [&](vector<int>& par, vector<int>& V, vector<int>& indptr) -> void {
        int N = len(par);
        vector<int> dist(N);
        For(i, 1, N) { dist[i] = dist[par[i]] + 1; }
        auto calc = [&](int L, int R, int sgn) -> void {
            int mx = *max_element(dist.begin() + L, dist.begin() + R);
            vector<long long> f(mx + 1);
            For(i, L, R) f[dist[i]]++;
            f = convolution(f, f);
            For(i, len(f)) ANS[i] += sgn * f[i];
        };
        calc(0, N, +1);
        For(k, 1, len(indptr) - 1) { calc(indptr[k], indptr[k + 1], -1); }
    };
    centroid_decomposition<0>(G, f);
    for (auto& x: ANS) x /= 2;
    ANS.erase(ANS.begin());
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
