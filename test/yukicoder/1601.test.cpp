#define PROBLEM "https://yukicoder.me/problems/no/1601"
#include "library/template.hpp"
#include "library/graph/shortestpath/dijkstra.hpp"
#include "library/graph/reverse_graph.hpp"

void solve() {
    LL(N, M);
    Graph<long long, 1> G(N + N);
    For(M) {
        LL(a, b, c, x);
        --a, --b;
        For(2) {
            swap(a, b);
            if (x == 0) {
                G.add(2 * a + 0, 2 * b + 0, c);
                G.add(2 * a + 1, 2 * b + 1, c);
            }
            if (x == 1) {
                G.add(2 * a + 0, 2 * b + 1, c);
                G.add(2 * a + 1, 2 * b + 1, c);
            }
        }
    }
    G.build();

    G = reverse_graph(G);

    auto [dist, par] = dijkstra<long long>(G, 2 * (N - 1) + 1);
    For(v, N - 1) print(dist[2 * v + 0]);
}

signed main() {
    solve();
    return 0;
}
