#define PROBLEM "https://yukicoder.me/problems/no/1776"
#include "library/template.hpp"
#include "library/graph/cycle/find_path_through_specified.hpp"

void solve() {
    LL(N, M);
    INT(X, Y, Z);
    --X, --Y, --Z;
    vector<vector<bool>> can(N, vector<bool>(N, 1));
    For(M) {
        LL(a, b);
        --a, --b;
        can[a][b] = can[b][a] = 0;
    }

    vector<int> path = {X};
    while (1) {
        vector<int> done(N);
        For(i, 1, len(path) - 1) { done[path[i]] = 1; }
        Graph<int, 0> G(N);
        For(b, N) For(a, b) {
            if (!can[a][b] || done[a] || done[b]) continue;
            G.add(a, b);
        }
        int s = path.back(), t = X;
        vector<int> need;
        for (auto& y: {Y, Z}) {
            if (y == s || y == t || done[y]) continue;
            need.emplace_back(y);
        }
        auto [L, nxt] = find_path_through_specified(G, s, t, need);
        if (L == -1) return print(-1);
        path.emplace_back(nxt);
        if (nxt == X) break;
    }

    for (auto& x: path) ++x;
    print(len(path) - 1);
    print(path);
}

signed main() {
    solve();
    return 0;
}
