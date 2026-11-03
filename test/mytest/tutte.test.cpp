#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/matching/maximum_matching_size.hpp"
#include "library/flow/bipartite_matching.hpp"

void test_bipartite() {
    For(N, 1, 50) {
        For(M, RNG(0, N * (N - 1) / 2 + 1)) {
            Graph<bool, 0> G(N);
            For(M) {
                int a = RNG(0, N);
                int b = RNG(0, N);
                if (a % 2 == b % 2) continue;
                G.add(a, b);
            }
            G.build();
            Bipartite_Matching<decltype(G)> X(G);
            int a = len(X.matching());
            int b = maximum_matching_size(G);
            assert(a == b);
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test_bipartite();
    solve();

    return 0;
}
