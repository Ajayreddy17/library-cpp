#define PROBLEM "https://yukicoder.me/problems/no/1641"
#include "library/template.hpp"
#include "library/graph/datastructure/tree_abelgroup.hpp"
#include "library/monoid/monoid_xor.hpp"

void solve() {
    LL(N, Q);
    Graph G(N);
    vector<int> A(N); read(A);
    G.read_tree();
    Tree tree(G);
    Tree_AbelGroup<decltype(tree), Monoid_Xor<int>, 0, 0, 1> TA(tree, A);
    For(Q) {
        LL(t, x, y);
        --x;
        if (t == 1) {
            TA.add(x, y);
        }
        if (t == 2) {
            print(TA.prod_subtree(x));
        }
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    long long T = 1;
    // LL(T);
    For(_, T) solve();

    return 0;
}
