#define PROBLEM "https://yukicoder.me/problems/no/1326"
#include "library/template.hpp"
#include "library/graph/connectivity/block_cut.hpp"
#include "library/graph/datastructure/tree_abelgroup.hpp"
#include "library/monoid/monoid_add.hpp"

void solve() {
    LL(N, M);
    Graph<int> G(N);
    G.read_graph(M);
    Block_Cut X(G);
    auto &BCT = X.BCT;
    Tree<decltype(X.BCT)> tree(X.BCT);

    vector<int> dat(tree.N);
    For(v, N) if (BCT.deg(v) > 1) dat[v] = 1;
    Tree_AbelGroup<decltype(tree), Monoid_Add<int>, 0, 1, 0> TA(tree, dat);

    LL(Q);
    For(Q) {
        LL(x, y);
        --x, --y;
        if (x == y) {
            print(0);
            continue;
        }
        long long k = TA.prod_path(x, y);
        if (BCT.deg(x) > 1) --k;
        if (BCT.deg(y) > 1) --k;
        print(k);
    }
}

signed main() {
    solve();
    return 0;
}
