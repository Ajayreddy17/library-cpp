#define PROBLEM  "https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification"
#include "library/template.hpp"
#include "library/graph/tree/subtree_hash.hpp"

void solve() {
    LL(N);
    Graph<int, 0> G(N);
    For(v, 1, N) {
        INT(p);
        G.add(p, v);
    }
    G.build();
    Tree<decltype(G)> tree(G);
    Subtree_Hash<decltype(tree)> X(tree);

    vector<long long> ANS(N);
    For(v, N) ANS[v] = X.get(v, 0);
    vector<long long> key = ANS;
    settify(key);
    for (auto&& x : ANS) x = (lower_bound(key.begin(), key.end(), x) - key.begin());
    print(*max_element(ANS.begin(), ANS.end()) + 1);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
