#define PROBLEM "https://judge.yosupo.jp/problem/bipartite_edge_coloring"
#include <library/template.hpp>
#include <library/graph/bipartite_edge_coloring.hpp>

void solve() {
    LL(L, R, M);
    Graph<bool, 0> G(L + R);
    For(M) {
        LL(a, b);
        G.add(a, L + b);
    }
    G.build();
    auto [C, color] = bipartite_edge_coloring(G);
    print(C);
    for (auto&& x : color) print(x);
}

signed main() {
    solve();
    return 0;
}
