#define PROBLEM "https://judge.yosupo.jp/problem/eulerian_trail_undirected"

#include <library/template.hpp>
#include <library/graph/euler_walk.hpp>

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M, 0, 0);
    auto [vs, es] = euler_walk(G);
    if (vs.empty()) { print("No"); return; }
    print("Yes");
    print(vs);
    print(es);
}

signed main() {
    INT(T);
    For(T) solve();
    return 0;
}
