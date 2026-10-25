#define PROBLEM "https://judge.yosupo.jp/problem/closest_pair"
#include "library/template.hpp"

#include "library/geo/closest_pair.hpp"

using P = Point<long long>;

void solve() {
  LL(N);
  VT(P, point, N);
  auto [i, j] = closest_pair(point);
  print(i, j);
}

signed main() {
  INT(T);
  For(T) solve();
  return 0;
}
