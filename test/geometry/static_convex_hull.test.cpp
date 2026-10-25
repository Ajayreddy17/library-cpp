#define PROBLEM "https://judge.yosupo.jp/problem/static_convex_hull"
#include "library/template.hpp"

#include "library/geo/convexhull.hpp"
#include "library/geo/base.hpp"

using P = Point<long long>;

void solve() {
  LL(N);
  VT(P, point, N);
  vector<int> I = ConvexHull(point);
  print(len(I));
  for (auto& i : I) print(point[i]);
}

signed main() {
  INT(T);
  For(T) solve();
}
