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
    auto f = [&](vector<int>& par, vector<int>& V, vector<int>& color) -> void {
        int N = len(par);
        vector<int> dist(N);
        For(i, 1, N) { dist[i] = dist[par[i]] + 1; }
        For(p, 3) {
            vector<long long> f(N), g(N);
            For(i, N) {
                if (color[i] == p) f[dist[i]]++;
                if (color[i] > p) g[dist[i]]++;
            }
            while (len(f) && f.back() == 0) POP(f);
            while (len(g) && g.back() == 0) POP(g);
            f = convolution(f, g);
            For(i, len(f)) ANS[i] += f[i];
        }
    };
    centroid_decomposition<2>(G, f);
    ANS.erase(ANS.begin());
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
