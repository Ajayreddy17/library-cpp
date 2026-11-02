#define PROBLEM "https://yukicoder.me/problems/no/1344"
#include "library/template.hpp"
#include "library/graph/shortestpath/warshall_floyd.hpp"

void solve() {
    LL(N, M);
    Graph<long long, 1> G(N);
    G.read_graph(M, 1);
    auto dist = warshall_floyd<long long>(G);
    For(v, N) {
        long long ANS = 0;
        For(w, N) if (dist[v][w] != infty<long long>) ANS += dist[v][w];
        print(ANS);
    }
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
