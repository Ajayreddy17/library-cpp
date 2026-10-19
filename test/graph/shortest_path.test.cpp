#define PROBLEM "https://judge.yosupo.jp/problem/shortest_path"

#include <library/template.hpp>

#include <library/graph/shortestpath/dijkstra.hpp>
#include <library/graph/restore_path.hpp>

void solve() {
    LL(N, M, s, t);
    Graph<long long, true> G(N);
    G.read_graph(M, 1, 0);

    auto [dist, par] = dijkstra<long long>(G, s);
    if (dist[t] == linf) return print(-1);

    vector<int> pth = restore_path(par, t);
    print(dist[t], len(pth) - 1);
    For(i, len(pth) - 1) print(pth[i], pth[i + 1]);
}

signed main() {
    solve();
    return 0;
}
