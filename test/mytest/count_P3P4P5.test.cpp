#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/count/count_P3_P4_P5.hpp"
#include "library/util/maspy_compat.hpp"

void test() {
  For(N, 20) {
    For(50) {
      Graph<int, 0> G(N);
      for (auto& [a, b] : random_graph<false>(N, true)) G.add(a, b);
      G.build();
      vector<vector<int>> adj(N, vector<int>(N));
      for (auto& e : G.edges) adj[e.frm][e.to] += 1, adj[e.to][e.frm] += 1;
      vector<long long> A(N), B(N), C(N);
      For(a, N) For(b, N) For(c, N) {
        int s = (1 << a) | (1 << b) | (1 << c);
        if (popcnt(s) != 3) continue;
        A[a] += adj[a][b] * adj[b][c];
      }
      For(a, N) For(b, N) For(c, N) For(d, N) {
        int s = (1 << a) | (1 << b) | (1 << c) | (1 << d);
        if (popcnt(s) != 4) continue;
        B[a] += adj[a][b] * adj[b][c] * adj[c][d];
      }
      For(a, N) For(b, N) For(c, N) For(d, N) For(e, N) {
        int s = (1 << a) | (1 << b) | (1 << c) | (1 << d) | (1 << e);
        if (popcnt(s) != 5) continue;
        C[a] += adj[a][b] * adj[b][c] * adj[c][d] * adj[d][e];
      }
      auto [X, Y, Z] = count_P3_P4_P5_pointwise(G);
      assert(A == X);
      assert(B == Y);
      assert(C == Z);
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
