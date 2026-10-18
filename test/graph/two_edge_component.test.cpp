#define PROBLEM "https://judge.yosupo.jp/problem/two_edge_connected_components"
#include <library/template.hpp>
#include <library/graph/two_edge_component.hpp>

void solve() {
    LL(N, M);
    Graph G(N);
    G.read_graph(M, false, 0);

    auto [C, comp] = two_edge_component(G);
    vector<vector<int>> ANS(C);
    For(v, N) ANS[comp[v]].emplace_back(v);
    print(len(ANS));
    for (auto&& ans: ANS) print(len(ans), ans);
}

signed main() {
    solve();

    return 0;
}
