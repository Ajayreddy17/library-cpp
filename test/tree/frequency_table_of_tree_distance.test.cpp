#define PROBLEM \
    "https://judge.yosupo.jp/problem/frequency_table_of_tree_distance"
#include <library/template.hpp>

#include <library/graph/tree_all_distances.hpp>

void solve() {
    LL(N);
    Graph<int> G(N);
    G.read_tree(0, 0);

    vector<long long> ANS = tree_all_distances(G);
    ANS.erase(ANS.begin());
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
