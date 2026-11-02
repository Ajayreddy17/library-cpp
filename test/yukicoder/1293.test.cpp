#define PROBLEM "https://yukicoder.me/problems/no/1293"
#include "library/template.hpp"
#include "library/graph/strongly_connected_component.hpp"

void solve() {
    LL(N, D, W);
    Graph<int, 1> G(N + N);
    For(D) {
        LL(a, b);
        --a, --b;
        G.add(2 * a, 2 * b);
        G.add(2 * b, 2 * a);
    }
    For(W) {
        LL(a, b);
        --a, --b;
        G.add(2 * a + 1, 2 * b + 1);
        G.add(2 * b + 1, 2 * a + 1);
    }
    For(v, N) G.add(2 * v, 2 * v + 1);
    G.build();

    auto [C, comp] = strongly_connected_component(G);
    auto DAG = scc_dag(G, C, comp);
    vector<long long> DP(C);
    For(v, N) { DP[comp[2 * v + 0]]++; }
    For(c, C) {
        for (auto&& e: DAG[c]) { DP[e.to] += DP[c]; }
    }

    long long ANS = 0;
    For(v, N) ANS += DP[comp[2 * v + 1]] - 1;

    print(ANS);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
