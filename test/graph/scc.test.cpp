#define PROBLEM "https://judge.yosupo.jp/problem/scc"
#include <library/template.hpp>

#include <library/graph/base.hpp>
#include <library/graph/strongly_connected_component.hpp>

void solve() {
    LL(N, M);
    Graph<int, 1> G(N);
    G.read_graph(M, 0, 0);

    auto [C, comp] = strongly_connected_component(G);
    vector<vector<int>> ANS(C);
    For(v, N) ANS[comp[v]].emplace_back(v);
    print(len(ANS));
    for (auto&& C: ANS) print(len(C), C);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
