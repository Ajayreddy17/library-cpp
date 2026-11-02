#define PROBLEM "https://yukicoder.me/problems/no/1479"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/flow/bipartite_matching.hpp"

void solve() {
    LL(H, W);
    long long LIM = 500000;
    vector<vector<pair<long long, long long>>> dat(LIM + 1);
    For(x, H) For(y, W) {
        LL(v);
        dat[v].emplace_back(x, y);
    }
    long long ANS = 0;
    For(v, 1, LIM + 1) {
        auto& XY = dat[v];
        if (len(XY) == 0) continue;
        vector<long long> X, Y;
        for (auto&& [x, y] : XY) {
            X.emplace_back(x);
            Y.emplace_back(y);
        }
        settify(X);
        settify(Y);
        Graph G(len(X) + len(Y));
        for (auto&& [x, y] : XY) {
            x = lower_bound(X.begin(), X.end(), x) - X.begin();
            y = lower_bound(Y.begin(), Y.end(), y) - Y.begin();
            G.add(x, len(X) + y);
        }
        G.build();
        Bipartite_Matching BM(G);
        auto cov = BM.vertex_cover();
        ANS += len(cov);
        // print(v, len(cov));
    }
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
