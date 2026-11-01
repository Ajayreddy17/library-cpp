#define PROBLEM  "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_12_B"
#include "library/template.hpp"
#include "library/graph/shortestpath/dial.hpp"

void solve() {
  LL(N);
  Graph<int, 1> G(N);
  For(v, N) {
    LL(u);
    assert(u == v);
    LL(k);
    For(k) {
      LL(a, b);
      G.add(v, a, b);
    }
  }
  G.build();
  auto [dist, par] = dial(G, 0);
  For(v, N) print(v, dist[v]);
}

signed main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
