#define PROBLEM "https://judge.yosupo.jp/problem/vertex_add_subtree_sum"
#include <library/template.hpp>

#include <library/graph/datastructure/tree_monoid.hpp>
#include <library/monoid/monoid_add.hpp>

void solve() {
    LL(N, Q);
    VT(long long, A, N);
    Graph<int, 1> G(N);
    For(v, 1, N) {
        LL(p);
        G.add(p, v);
    }
    G.build();

    Tree tree(G);
    using Mono = Monoid_Add<long long>;

    Tree_Monoid<decltype(tree), Mono, false> TM(tree, A);

    For(_, Q) {
        LL(t);
        if (t == 0) {
            LL(v, x);
            A[v] += x;
            TM.set(v, A[v]);
        } else {
            LL(u);
            print(TM.prod_subtree(u));
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
