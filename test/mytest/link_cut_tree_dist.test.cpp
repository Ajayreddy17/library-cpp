#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/datastructure/link_cut_tree.hpp"
#include "library/graph/datastructure/lct_node_commutative_monoid.hpp"
#include "library/monoid/monoid_add.hpp"

using Node = LCT_Node_Commutative_Monoid<Monoid_Add<long long>>;

// dist / depth must not change the root: lca and get_root stay valid afterwards
void test() {
    For(iter, 300) {
        int n = RNG(1, 30);
        vector<int> par(n, -1), dep(n, 0);
        Link_Cut_Tree<Node> LCT(n);
        For(i, 1, n) {
            par[i] = RNG(0, i), dep[i] = dep[par[i]] + 1;
            LCT.link(i, par[i]);
        }
        auto lca = [&](int a, int b) {
            while (dep[a] > dep[b]) a = par[a];
            while (dep[b] > dep[a]) b = par[b];
            while (a != b) a = par[a], b = par[b];
            return a;
        };
        For(q, 100) {
            int u = RNG(0, n), v = RNG(0, n);
            int c = lca(u, v);
            assert(LCT.dist(u, v) == dep[u] + dep[v] - 2 * dep[c]);
            assert(LCT.depth(u) == dep[u]);
            int a = RNG(0, n), b = RNG(0, n);
            assert(LCT.lca(a, b) == lca(a, b));
            assert(LCT.get_root(a) == 0);
        }
    }
}

void solve() {
    LL(a, b);
    print(a + b);
}

signed main() {
    test();
    solve();
    return 0;
}
