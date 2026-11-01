#define PROBLEM "https://yukicoder.me/problems/no/1745"
#include "library/template.hpp"
#include "library/flow/bipartite_matching.hpp"

void solve() {
  LL(L, R, M);
  Graph<int, 0> G(L + R);
  vector<pair<long long, long long>> XY(M); read(XY);
  for (auto&& [x, y] : XY) {
    --x, --y;
    y += L;
  }
  for (auto&& [x, y] : XY) G.add(x, y);
  G.build();

  Bipartite_Matching<decltype(G)> X(G);

  auto [K, W] = X.DM_decomposition();
  for (auto&& [x, y] : XY) print(Yes[W[x] == W[y]]);
}

signed main() {
  solve();
  return 0;
}
