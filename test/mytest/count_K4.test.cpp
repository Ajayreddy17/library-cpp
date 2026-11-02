#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/count/count_K4.hpp"

void test() {
    For(N, 20) {
        For(1000) {
            Graph<int, 0> G(N);
            for (auto& [a, b]: random_graph<false>(N, true)) G.add(a, b);
            G.build();
            vector<vector<int>> adj(N, vector<int>(N));
            for (auto& e: G.edges) adj[e.frm][e.to] = adj[e.to][e.frm] = 1;
            long long ans = 0;
            For(d, N) For(c, d) For(b, c) For(a, b) {
                long long n = 0;
                n += adj[a][b];
                n += adj[a][c];
                n += adj[a][d];
                n += adj[b][c];
                n += adj[b][d];
                n += adj[c][d];
                ans += n == 6;
            }
            long long x = count_K4(G);
            assert(ans == x);
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();
    return 0;
}
