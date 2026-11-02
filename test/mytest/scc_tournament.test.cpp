#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/strongly_connected_component.hpp"
#include "library/graph/scc_tournament_by_indegrees.hpp"
#include "library/random/base.hpp"

void test() {
    For(N, 1, 50) {
        For(100) {
            Graph<int, 1> G(N);
            For(b, N) For(a, b) {
                int x = RNG(0, 2);
                if (x) G.add(a, b);
                if (!x) G.add(b, a);
            }
            G.build();

            auto [indeg, outdeg] = G.deg_array_inout();
            assert(strongly_connected_component(G) ==
                          scc_tournament_by_indegrees(indeg));
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();
    return 0;
}
