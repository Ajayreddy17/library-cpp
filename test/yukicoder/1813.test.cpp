#define PROBLEM "https://yukicoder.me/problems/no/1813"
#include "library/template.hpp"
#include "library/graph/strongly_connected_component.hpp"

void solve() {
    LL(N, M);
    Graph<int, 1> G(N);
    G.read_graph(M);

    auto [C, comp] = strongly_connected_component(G);
    if (C == 1) return print(0);
    auto DAG = scc_dag<decltype(G)>(G, C, comp);
    auto [indeg, outdeg] = DAG.deg_array_inout();
    long long a = 0, b = 0;
    for (auto&& x: indeg)
        if (x == 0) ++a;
    for (auto&& x: outdeg)
        if (x == 0) ++b;
    print(max(a, b));
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
