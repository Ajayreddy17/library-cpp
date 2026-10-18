#define PROBLEM "https://judge.yosupo.jp/problem/minimum_spanning_tree"
#include <library/template.hpp>

#include <library/graph/minimum_spanning_tree.hpp>

void solve() {
    LL(N, M);
    Graph<int, 0> G(N);
    G.read_graph(M, 1, 0);
    auto [cost, in_mst, MST] = minimum_spanning_tree<long long>(G);
    vector<int> ANS;
    For(i, M) if (in_mst[i]) ANS.emplace_back(i);
    print(cost);
    print(ANS);
}

signed main() {
    solve();
    return 0;
}
