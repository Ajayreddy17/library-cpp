#define PROBLEM \
    "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_subtree_sum"
#include <library/template.hpp>

#include <library/graph/datastructure/link_cut_tree.hpp>
#include <library/monoid/monoid_add.hpp>
#include <library/graph/datastructure/lct_node_commutative_monoid.hpp>

using Node = LCT_Node_Commutative_Monoid<Monoid_Add<long long>, true>;

void solve() {
    LL(N, Q);
    Link_Cut_Tree<Node> LCT(N);

    VT(long long, A, N);
    For(i, N) { LCT.set(i, A[i]); }
    For(N - 1) {
        INT(a, b);
        LCT.link(a, b);
    }

    For(q, Q) {
        LL(t);
        if (t == 0) {
            LL(a, b, c, d);
            LCT.cut(a, b);
            LCT.link(c, d);
        }
        if (t == 1) {
            LL(i, x);
            A[i] += x;
            LCT.set(i, A[i]);
        }
        if (t == 2) {
            LL(a, b);
            print(LCT.prod_subtree(a, b));
        }
    }
}

signed main() {
    solve();
    return 0;
}
