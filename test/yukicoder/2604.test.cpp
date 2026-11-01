#define PROBLEM "https://yukicoder.me/problems/no/2604"
#include "library/template.hpp"
#include "library/flow/mincostflow.hpp"

void solve() {
  LL(K, N, M);
  vector<int> A(K); read(A);
  for (auto& a : A) --a;
  vector<int> B(N); read(B);

  int s = N, t = N + 1;
  Min_Cost_Flow<long long, long long, false> G(N + 2, s, t);

  for (auto& a : A) G.add(s, a, 1, 0);
  For(i, N) G.add(i, t, B[i], 0);

  For(M) {
    LL(a, b, c);
    --a, --b;
    G.add(a, b, K, c);
    G.add(b, a, K, c);
  }
  auto [a, b] = G.flow();
  print(b);
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
