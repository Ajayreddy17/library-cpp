#define PROBLEM "https://yukicoder.me/problems/no/1069"
#define ERROR 0.001
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/shortestpath/k_shortest_path.hpp"

void solve() {
  using Re = double;
  LL(N, M, K);
  LL(s, t);
  --s, --t;
  vector<pair<long long, long long>> P(N); read(P);
  Graph<double, 1> G(N);
  For(M) {
    LL(i, j);
    --i, --j;
    auto [xi, yi] = P[i];
    auto [xj, yj] = P[j];
    Re dx = xi - xj, dy = yi - yj;
    Re d = sqrt(dx * dx + dy * dy);
    G.add(i, j, d);
    G.add(j, i, d);
  }
  G.build();
  auto res = K_shortest_path<double>(G, s, t, K);
  for (auto&& [x, vs, es]: res) print(x);
  For(K - len(res)) print(-1);
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
