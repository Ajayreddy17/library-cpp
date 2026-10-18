#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include <library/template.hpp>

#include <library/graph/base.hpp>
#include <library/graph/fast_lca.hpp>

void solve() {
    INT(N, Q);
    Graph<int, 1> G(N);
    For(v, 1, N) {
        INT(p);
        G.add(p, v);
    }
    G.build();

    Tree<decltype(G)> tree(G);
    Fast_LCA<decltype(tree)> LCA(tree);

    For(Q) {
        INT(a, b);
        print(LCA.LCA(a, b));
    }
}

signed main() {
    solve();
    return 0;
}
