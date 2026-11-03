#define PROBLEM "https://yukicoder.me/problems/no/1775"
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

    Graph<int, 0> G(N);
    For(a, N) For(b, a) if (can[a][b]) G.add(a, b);
    G.build();
    vector<int> need = {Y, Z};
    auto [L, nxt] = find_path_through_specified(G, X, X, need);
    print(L);
}

signed main() {
    solve();
    return 0;
}
