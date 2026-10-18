#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_path_sum"
#include <library/template.hpp>

#include <library/graph/datastructure/tree_monoid.hpp>
#include <library/monoid/monoid_add.hpp>

void solve() {
    LL(N, Q);
    VT(long long, A, N);
    Graph G(N);
    G.read_tree(0, 0);

    Tree tree(G);
    const bool is_edge = false;
    using Mono = Monoid_Add<long long>;
    Tree_Monoid<decltype(tree), Mono, is_edge> TM(tree, A);

    For(_, Q) {
        LL(t);
        if (t == 0) {
            LL(v, x);
            A[v] += x;
            TM.set(v, A[v]);
        } else {
            LL(u, v);
            print(TM.prod_path(u, v));
        }
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
