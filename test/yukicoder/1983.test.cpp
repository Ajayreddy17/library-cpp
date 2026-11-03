#define PROBLEM "https://yukicoder.me/problems/no/1983"
#include "library/template.hpp"
#include "library/graph/connectivity/two_edge_component.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

void solve() {
    LL(N, M, Q);
    Graph<int, 0> G(N);
    G.read_graph(M);
    auto [C, comp] = two_edge_component(G);
    UnionFind uf(N);
    for (auto&& e: G.edges) {
        int a = e.frm, b = e.to;
        if (comp[a] != comp[b]) uf.merge(a, b);
    }
    For(Q) {
        LL(a, b);
        print(Yes[uf[--a] == uf[--b]]);
    }
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
