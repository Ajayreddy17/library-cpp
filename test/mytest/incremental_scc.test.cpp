#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/connectivity/incremental_scc.hpp"
#include "library/random/random_graph.hpp"

void test() {
    For(N, 1, 30) {
        For(50) {
            auto edges = random_graph<true>(N, false);

            int M = len(edges);
            vector<vector<int>> comp(M + 1);
            For(t, M + 1) {
                Graph<int, 1> G(N);
                For(i, t) {
                    auto [a, b] = edges[i];
                    G.add(a, b);
                }
                G.build();
                comp[t] = strongly_connected_component(G).second;
            }
            Graph<int, 1> G(N);
            for (auto& [a, b]: edges) G.add(a, b);
            G.build();
            auto time = incremental_scc(G);

            For(i, M) {
                auto [a, b] = edges[i];
                int t = time[i];
                assert(i < t);
                if (t == iinf) {
                    assert(comp[M][a] != comp[M][b]);
                } else {
                    assert(comp[t][a] == comp[t][b]);
                    if (i + 1 < t) assert(comp[t - 1][a] != comp[t - 1][b]);
                }
            }
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
