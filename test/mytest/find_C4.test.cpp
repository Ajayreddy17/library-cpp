#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/count/count_C3_C4.hpp"
#include "library/graph/find_C4.hpp"

void test() {
  For(N, 20) {
    For(50) {
      Graph<int, 0> G(N);
      for (auto& [a, b]: random_graph<false>(N, true)) G.add(a, b);
      G.build();
      vector<vector<int>> adj(N, vector<int>(N));
      for (auto& e: G.edges) adj[e.frm][e.to] += 1, adj[e.to][e.frm] += 1;
      auto [a, b, c, d] = find_C4(G);
      long long cnt = count_C3_C4(G).second;
      if (cnt == 0) {
        assert(a == -1);
      } else {
        assert(adj[a][b] && adj[b][c] && adj[c][d] && adj[d][a]);
      }
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
