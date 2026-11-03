#define PROBLEM "https://yukicoder.me/problems/no/17"
#include "library/template.hpp"
#include "library/graph/shortestpath/warshall_floyd.hpp"

void solve() {
    LL(N);
    vector<long long> COST(N); read(COST);
    Graph<long long> G(N);
    LL(M);
    G.read_graph(M, 1, 0);

    auto dist = warshall_floyd<long long>(G);

    long long ANS = linf;
    For(i, 1, N - 1) For(j, 1, N - 1) if (i != j) {
        chmin(ANS, dist[0][i] + dist[i][j] + dist[j][N - 1] + COST[i] + COST[j]);
    }
    print(ANS);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(_, T) solve();

    return 0;
}
