#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/base.hpp"
#include "library/graph/count/count_independent_set.hpp"
#include "library/util/bit_utils.hpp"

void test() {
  For(100) {
    For(n, 10) {
      Graph<int, 0> G(n);
      for (auto& [a, b]: random_graph<0>(n, true)) G.add(a, b);
      G.build();
      vector<vector<int>> adj(n, vector<int>(n));

      for (auto&& e: G.edges) adj[e.frm][e.to] = adj[e.to][e.frm] = 1;
      auto X = count_independent_set<decltype(G)>(G);
      auto Y = count_independent_set_by_size<decltype(G)>(G);
      For(s, 1 << n) {
        bool ok = 1;
        For(i, n) For(j, n) {
          if ((s >> i & 1) && (s >> j & 1) && adj[i][j]) ok = 0;
        }
        if (!ok) continue;
        --X;
        --Y[popcount(s)];
      }
      assert(X == 0);
      assert(len(Y) == n + 1);
      For(i, n + 1) assert(Y[i] == 0);
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
