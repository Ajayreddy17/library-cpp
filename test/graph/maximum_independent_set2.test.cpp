#define PROBLEM "https://judge.yosupo.jp/problem/maximum_independent_set"
#include "library/template.hpp"
#include "library/graph/maximum_independent_set_weighted.hpp"

void solve() {
  LL(N, M);
  Graph<int, 0> G(N);
  G.read_graph(M, 0, 0);
  vector<int> wt(N, 1);
  vector<int> V = maximum_independent_set_weighted(G, wt);
  print(len(V));
  print(V);
}

signed main() {
  solve();
  return 0;
}
