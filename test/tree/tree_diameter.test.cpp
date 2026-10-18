#define PROBLEM "https://judge.yosupo.jp/problem/tree_diameter"
#include <library/template.hpp>
#include <library/graph/tree_diameter.hpp>

void solve() {
    LL(N);
    Graph<long long> G(N);
    G.read_tree(1, 0);
    auto [diam, P] = tree_diameter<long long>(G);
    print(diam, len(P));
    print(P);
}

signed main() {
    solve();

    return 0;
}
