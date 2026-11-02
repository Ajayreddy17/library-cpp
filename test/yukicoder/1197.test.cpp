#define PROBLEM "https://yukicoder.me/problems/no/1197"
#include "library/template.hpp"
#include "library/graph/tree.hpp"
#include "library/acted_monoid/am_sum_add.hpp"
#include "library/graph/datastructure/lazy_tree_monoid.hpp"

void solve() {
    LL(N, K, Q);
    vector<int> C(K); read(C);
    for (auto&& v: C) --v;

    Graph<int, 0> G(N);
    G.read_tree();

    Tree<decltype(G)> tree(G);
    using AM = ActedMonoid_Sum_Add<long long>;

    Lazy_Tree_Monoid<decltype(tree), AM, 0> X(tree);

    For(i, K) { X.apply_path(0, C[i], 1); }

    long long d_sm = 0;
    auto& dep = tree.depth;
    For(i, K) d_sm += dep[C[i]];

    bool out = 0;

    For(Q) {
        LL(t);
        if (t == 1) {
            LL(k, d);
            --k;
            --d;
            d_sm -= dep[C[k]];
            X.apply_path(0, C[k], -1);
            C[k] = d;
            d_sm += dep[C[k]];
            X.apply_path(0, C[k], 1);
        }
        if (t == 2) {
            LL(v);
            --v;
            long long ANS = dep[v] * K + d_sm;
            ANS -= 2 * X.prod_path(0, v);
            ANS += 2 * K;
            print(ANS);
            out = 1;
        }
    }
    if (!out) print();
}

signed main() {
    solve();
    return 0;
}
