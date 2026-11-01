#define PROBLEM "https://yukicoder.me/problems/no/1602"
#include "library/template.hpp"
#include "library/monoid/monoid_xor.hpp"
#include "library/graph/shortestpath/nonzero_group_product_shortest_path.hpp"

void solve() {
  INT(N, M, K);
  Graph<int, 0> G(N);
  vector<uint32_t> label(M);
  For(i, M) {
    INT(a, b, c);
    --a, --b;
    STR(X);
    uint32_t v = 0;
    For(j, K) if (X[j] == '1') v |= uint32_t(1) << j;
    G.add(a, b, c);
    label[i] = v;
  }
  G.build();

  using Mono = Monoid_Xor<uint32_t>;
  auto ANS = nonzero_group_product_shortest_path<long long, Mono>(G, label, N - 1);
  For(v, N - 1) {
    long long ans = ANS[v];
    if (ans == infty<long long>) ans = -1;
    print(ans);
  }
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
