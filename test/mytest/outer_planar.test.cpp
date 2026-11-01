#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/random_graph.hpp"
#include "library/graph/outerplanar.hpp"
#include "library/util/maspy_compat.hpp"

void test() {
  int N = RNG(3, 100);
  auto edges = random_outerplanar(N);
  Graph<int, 0> G(N);
  for (auto& [a, b] : edges) G.add(a, b);
  G.build();
  auto C = hamiltonian_cycle_outerplanar(G);
  assert(!C.empty());
  assert(check_outerplanar(C, G));
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  For(10000) test();
  solve();
}
