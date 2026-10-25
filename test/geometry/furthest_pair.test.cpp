#define PROBLEM "https://judge.yosupo.jp/problem/furthest_pair"
#include "library/template.hpp"

#include "library/geo/furthest_pair.hpp"

using P = Point<long long>;

void solve() {
  INT(N);
  VT(P, point, N);
  auto [a, b] = furthest_pair(point);
  print(a, b);
}

signed main() {
  INT(T);
  For(T) solve();
  return 0;
}
