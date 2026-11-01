#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2636"
#include "library/template.hpp"
#include "library/graph/datastructure/incremental_centroid.hpp"
#include "library/util/maspy_compat.hpp"

void solve() {
  LL(N);
  Graph<long long, 1> G(N);
  For(v, 1, N) {
    LL(p, d);
    G.add(--p, v, d);
  }
  G.build();

  Tree<decltype(G)> tree(G);
  Incremental_Centroid<decltype(tree)> X(tree);

  long long ans = 0;

  For(v, N) {
    int cent = X.cent;
    X.add(v);
    int to = X.cent;
    ans += tree.dist_weighted(cent, v);
    ans -= tree.dist_weighted(cent, to);
    print(ans);
  }
}

signed main() {
  solve();
  return 0;
}
