#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/graph/tree/dsu_on_tree.hpp"

// distinct colours in every subtree; callbacks passed both inline and as named variables
void test() {
    For(iter, 500) {
        int n = RNG(1, 40), C = RNG(1, 6);
        Graph<int, 0> G(n);
        For(i, 1, n) G.add(RNG(0, i), i);
        G.build();
        Tree<decltype(G)> T(G);
        vector<int> col(n);
        For(i, n) col[i] = RNG(0, C);

        vector<int> expected(n);
        For(v, n) {
            set<int> S;
            For(i, T.LID[v], T.RID[v]) S.insert(col[T.V[i]]);
            expected[v] = len(S);
        }

        vector<int> cnt(C), hist, ans(n, -1);
        int distinct = 0;
        DSU_on_Tree(
            T, [&](int v) { if (cnt[col[v]]++ == 0) ++distinct; hist.push_back(col[v]); },
            [&](int v) { ans[v] = distinct; },
            [&]() { for (int c: hist) --cnt[c]; hist.clear(); distinct = 0; });
        assert(ans == expected);

        ans.assign(n, -1);
        auto add = [&](int v) { if (cnt[col[v]]++ == 0) ++distinct; hist.push_back(col[v]); };
        auto query = [&](int v) { ans[v] = distinct; };
        auto reset = [&]() { for (int c: hist) --cnt[c]; hist.clear(); distinct = 0; };
        DSU_on_Tree(T, add, query, reset);
        assert(ans == expected);
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
