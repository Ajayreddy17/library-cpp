#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2251"
#include "library/template.hpp"
#include "library/graph/shortestpath/dijkstra.hpp"
#include "library/graph/matching/maximum_antichain.hpp"

void solve(long long N, long long M, long long L) {
    vector<vector<long long>> dist(N, vector<long long>(N));
    {
        Graph<long long> G(N);
        G.read_graph(M, 1, 0);
        For(v, N) { dist[v] = dijkstra<long long>(G, v).first; }
    }
    vector<pair<long long, long long>> PT(L); read(PT);
    N = L;
    Graph<int, 1> G(N);
    For(a, N) For(b, N) {
        if (a == b) continue;
        auto [pa, ta] = PT[a];
        auto [pb, tb] = PT[b];
        if (ta + dist[pa][pb] <= tb) G.add(a, b);
    }
    G.build();
    auto A = maximum_antichain(G);
    print(len(A));
}

signed main() {
    while (1) {
        LL(N, M, L);
        if (N + M + L == 0) break;
        solve(N, M, L);
    }
    return 0;
}
