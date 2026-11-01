#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"
#include "library/template.hpp"
#include "library/flow/bipartite_matching.hpp"

void solve() {
  LL(L, R, M);
  Graph G(L + R);
  For(M) {
    LL(a, b);
    G.add(a, b + L);
  }
  G.build();
  Bipartite_Matching BM(G);

  auto match = BM.matching();
  print(len(match));
  for (auto&& [a, b] : match) print(a, b - L);
}

signed main() {
  solve();

  return 0;
}
