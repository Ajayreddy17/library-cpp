#define PROBLEM "https://yukicoder.me/problems/no/2295"
#include "library/template.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/datastructure/link_cut_tree.hpp"
#include "library/graph/datastructure/lct_node_commutative_monoid.hpp"
#include "library/monoid/monoid_max.hpp"

using Node = LCT_Node_Commutative_Monoid<Monoid_Max<int>>;

using mint = modint998;

void solve() {
    LL(N, X, Q);

    long long nxt = N;
    UnionFind uf(N + N - 1);
    // root ごとに、直径頂点番号
    vector<pair<int, int>> dat(N + N - 1);
    For(i, N + N - 1) dat[i] = {i, i};
    Link_Cut_Tree<Node> tree(2 * N - 1);

    vector<mint> dp(N + N - 1);

    auto Q1 = [&](long long a, long long b, long long val) -> void {
        if (uf[a] == uf[b]) return;
        tree.set(nxt, val);
        tree.link(a, nxt);
        tree.link(nxt, b);
        nxt++;

        a = uf[a], b = uf[b];
        long long xa = uf.size(a), xb = uf.size(b);
        mint ans = dp[a] + dp[b] + mint(xa * xb) * mint(val);
        uf.merge(a, b);
        dp[uf[a]] = ans;
    };

    For(Q) {
        LL(t);
        if (t == 1) {
            LL(v, val);
            Q1(X, v, val);
        }
        if (t == 2) {
            LL(u, v);
            if (uf[u] != uf[v]) {
                print(-1);
            } else {
                long long d = tree.prod_path(u, v);
                if (u == v) d = 0;
                print(d);
                X += d;
                X %= N;
            }
        }
        if (t == 3) {
            LL(v);
            print(dp[uf[v]]);
        }
        if (t == 4) {
            LL(val);
            X += val;
            X %= N;
        }
    }
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
