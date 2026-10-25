#define PROBLEM "https://judge.yosupo.jp/problem/convex_layers"
#include "library/template.hpp"

#include "library/geo/convex_layers.hpp"

using P = Point<long long>;

void solve() {
  LL(N);
  vector<P> A(N);
  For(i, N) read(A[i]);
  auto layer = convex_layers(A);
  vector<int> ANS(N);
  For(i, len(layer)) {
    for (auto& j : layer[i]) ANS[j] = 1 + i;
    vector<P> X = rearrange(A, layer[i]);
    if (len(X) <= 2) continue;
    For(a, len(X)) {
      int b = (a + 1) % len(X);
      int c = (b + 1) % len(X);
      assert(ccw(X[a], X[b], X[c]) >= 0);
    }
  }
  for (auto& x : ANS) print(x);
}

signed main() {
  solve();
  return 0;
}
