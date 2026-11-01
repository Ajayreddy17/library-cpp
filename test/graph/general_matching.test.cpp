#define PROBLEM "https://judge.yosupo.jp/problem/general_matching"
#include "library/template.hpp"
#include "library/graph/maximum_matching.hpp"
#include "library/graph/maximum_matching_size.hpp"

void solve() {
  LL(N, M);
  Graph<int, 0> G(N);
  G.read_graph(M, 0, 0);

  auto [ans, mu] = maximum_matching(G);

  vector<pair<int, int>> ANS;
  For(v, N) if (v < mu[v]) ANS.emplace_back(v, mu[v]);
  print(ans);
  for (auto&& x: ANS) print(x);
  assert(ans == maximum_matching_size(G));
}

signed main() {
  solve();
  return 0;
}
