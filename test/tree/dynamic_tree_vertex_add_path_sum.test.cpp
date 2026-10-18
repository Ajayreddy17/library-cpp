#define PROBLEM \
    "https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum"
#include <library/template.hpp>

#include <library/graph/datastructure/link_cut_tree.hpp>
#include <library/graph/datastructure/lct_node_commutative_monoid.hpp>
#include <library/monoid/monoid_add.hpp>

using Node = LCT_Node_Commutative_Monoid<Monoid_Add<long long>>;

void solve() {
    LL(N, Q);
    VT(uint64_t, A, N);
    Link_Cut_Tree<Node> LCT(N);
    For(i, N) LCT.set(i, A[i]);
    For(N - 1) {
        INT(a, b);
        LCT.link(a, b);
    }
    For(Q) {
        LL(t);
        if (t == 0) {
            LL(a, b, c, d);
            LCT.cut(a, b), LCT.link(c, d);
        }
        if (t == 1) {
            LL(i);
            uint32_t x;
            read(x);
            A[i] += x;
            LCT.set(i, A[i]);
        }
        if (t == 2) {
            LL(a, b);
            uint64_t ans = LCT.prod_path(a, b);
            print(ans);
        }
    }
}

signed main() {
    solve();
    return 0;
}
