#define PROBLEM "https://judge.yosupo.jp/problem/counting_eulerian_circuits"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/count/BEST.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void solve() {
  LL(N, M);
  Graph<int, 1> G(N);
  G.read_graph(M, 0, 0);
  mint ANS = BEST_theorem<mint, false>(G);
  print(ANS);
}

signed main() {
  solve();
  return 0;
}
