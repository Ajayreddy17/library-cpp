#define PROBLEM  "https://judge.yosupo.jp/problem/counting_spanning_tree_undirected"
#include "library/template.hpp"
#include "library/graph/count/count_spanning_tree.hpp"
#include "library/mod/modint.hpp"

using mint = modint998;

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M, 0, 0);
    mint ANS = count_spanning_tree_undirected<mint, false>(G);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
