#define PROBLEM "https://yukicoder.me/problems/no/1744"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/flow/bipartite_matching.hpp"

void solve() {
    LL(N, M, L);
    using P = pair<int, int>;
    vector<P> XY(L); read(XY);
    Graph<bool, 0> G(N + M);
    for (auto&& [x, y] : XY) {
        --x, --y;
        y += N;
        G.add(x, y);
    }
    G.build();
    Bipartite_Matching<decltype(G)> BM(G);

    auto [K, W] = BM.DM_decomposition();
    vector<int> CNT(K + 1);

    for (auto&& [x, y] : XY)
        if (W[x] == W[y]) CNT[W[x]]++;
    for (auto&& [x, y] : XY) {
        print(Yes[!(W[x] == W[y] && CNT[W[x]] == 1)]);
    }
}

signed main() {
    solve();
    return 0;
}
