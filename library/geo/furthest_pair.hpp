#ifndef AJAY_FURTHEST_PAIR
#define AJAY_FURTHEST_PAIR

#include "library/geo/base.hpp"
#include "library/geo/convexhull.hpp"

namespace mitsuha{
template <typename T>
pair<int, int> furthest_pair(vector<Point<T>> point) {
  T best = -1;
  pair<int, int> ANS = {-1, -1};

  auto upd = [&](int i, int j) -> void {
    Point<T> p = point[i] - point[j];
    long long d = p.dot(p);
    if (chmax(best, d)) ANS = {i, j};
  };
  upd(0, 1);

  auto I = ConvexHull(point);
  int n = len(I);
  if (n == 1) return ANS;
  if (n == 2) {
    return {I[0], I[1]};
  }
  /*
  直径と垂直な平行線 2 本で凸包をはさめる
  平行線 2 本ではさんで引っかかる 2 点が候補
  p[i]p[i+1] の反対側を候補とすればよい
  */
  For(i, n) I.emplace_back(I[i]);

  vector<Point<T>> C = rearrange(point, I);
  long long j = 1;
  For(i, n) {
    chmax(j, i);
    while (j < 2 * n && (C[i + 1] - C[i]).det(C[j + 1] - C[j]) > 0) ++j;
    upd(I[i], I[j]);
  }
  return ANS;
}

} // namespace mitsuha
#endif // AJAY_FURTHEST_PAIR
