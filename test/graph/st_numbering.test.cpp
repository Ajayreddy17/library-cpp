#define PROBLEM "https://judge.yosupo.jp/problem/st_numbering"
#include "library/template.hpp"
#include "library/graph/st_numbering.hpp"

void solve() {
  LL(N, M, s, t);
  Graph<int, 0> G(N);
  G.read_graph(M, 0, 0);
  auto p = st_numbering(G, s, t);
  if (p.empty()) {
    print(Yes[!(1)]);
  } else {
    print(Yes[1]);
    print(p);
  }
}

signed main() {
  INT(T);
  For(T) solve();
  return 0;
}
