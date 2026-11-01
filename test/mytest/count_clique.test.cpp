#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/count/count_clique.hpp"
#include "library/util/maspy_compat.hpp"

void test() {
  For(N, 15) {
    For(100) {
      Graph<int, 0> G(N);
      for (auto& [a, b]: random_graph<0>(N, true)) G.add(a, b);
      G.build();
      vector<int> nbd(N);
      for (auto& e: G.edges) {
        nbd[e.frm] |= 1 << e.to;
        nbd[e.to] |= 1 << e.frm;
      }
      uint64_t n = 0;
      For(s, 1 << N) {
        int ok = 1;
        For(j, N) For(i, j) {
          if ((s >> i & 1) && (s >> j & 1)) {
            if (!(nbd[i] >> j & 1)) ok = 0;
          }
        }
        n += ok;
      }
      assert(n == count_clique(G));
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
