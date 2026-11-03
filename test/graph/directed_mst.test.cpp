#define PROBLEM "https://judge.yosupo.jp/problem/directedmst"
#include "library/template.hpp"
#include "library/graph/spanning/directed_mst.hpp"

void solve() {
    INT(N, M, s);
    Graph<long long, 1> G(N);
    G.read_graph(M, 1, 0);
    auto [cost, I] = directed_mst<decltype(G)>(G, s);
    vector<int> par(N, -1);
    par[s] = s;
    for (auto &idx : I) {
        auto &e = G.edges[idx];
        par[e.to] = e.frm;
    }
    print(cost);
    print(par);
}

signed main() {
    solve();
    return 0;
}
