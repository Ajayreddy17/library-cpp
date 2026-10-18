#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include <library/template.hpp>

#include <library/graph/tree.hpp>

void solve() {
    LL(N, Q);
    Graph<int, 1> G(N);
    For(v, 1, N) {
        LL(p);
        G.add(p, v);
    }
    G.build();
    Tree tree(G);

    For(_, Q) {
        LL(a, b);
        print(tree.LCA(a, b));
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();
    return 0;
}
