#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/maximum_matching.hpp"
#include "library/graph/maximum_matching_size.hpp"
#include "library/random/base.hpp"

void test() {
  For(N, 0, 100) {
    For(M, 0, 100) {
      if (N == 0 && M > 0) break;
      Graph<int, 0> G(N);
      For(M) {
        int a = RNG(0, N);
        int b = RNG(0, N);
        G.add(a, b);
      }
      G.build();
      int x = maximum_matching(G).first;
      int y = maximum_matching_size(G);
      assert(x == y);
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
