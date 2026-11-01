#define PROBLEM "https://judge.yosupo.jp/problem/biconnected_components"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/block_cut.hpp"

void solve() {
  LL(N, M);
  Graph<int, 0> G(N);
  G.read_graph(M, 0, 0);

  Block_Cut X(G);
  auto T = X.BCT;

  int single = 0;
  For(v, N) if (G.deg(v) == 0)++ single;

  print(T.N - N + single);
  For(k, N, T.N) {
    vector<int> ANS;
    for (auto&& e : T[k]) ANS.emplace_back(e.to);
    print(len(ANS), ANS);
  }
  For(v, N) if (G.deg(v) == 0) print(1, v);
}

signed main() {
  solve();

  return 0;
}
