#define PROBLEM "https://yukicoder.me/problems/no/1254"
#include "library/template.hpp"
#include "library/graph/cycle/unicyclicgraph.hpp"

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    G.read_graph(N);
    UnicyclicGraph X(G);
    vector<int> ANS;
    For(i, N) {
        auto& e = G.edges[i];
        if (X.in_cycle[e.frm] && X.in_cycle[e.to]) ANS.emplace_back(i + 1);
    }
    print(len(ANS));
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
