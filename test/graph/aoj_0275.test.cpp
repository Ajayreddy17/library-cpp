#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0275"
#include "library/template.hpp"
#include "library/graph/connectivity/reachability.hpp"
#include "library/graph/shortestpath/dijkstra.hpp"

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M, 1);
    LL(a, b);
    --a, --b;

    auto [dist_a, par_a] = dijkstra<int>(G, a);
    auto [dist_b, par_b] = dijkstra<int>(G, b);

    Graph<int, 1> DG(N);
    long long d = dist_a[b];
    for (auto&& e : G.edges) {
        if (dist_a[e.frm] + e.cost + dist_b[e.to] == d) DG.add(e.frm, e.to);
        if (dist_b[e.frm] + e.cost + dist_a[e.to] == d) DG.add(e.to, e.frm);
    }
    DG.build();

    LL(Q);
    vector<pair<long long, long long>> query(Q); read(query);
    for (auto&& [a, b] : query) --a, --b;
    auto res = reachability(DG, query);
    for (auto&& yn : res) print(Yes[yn]);
}

signed main() {
    solve();
    return 0;
}
