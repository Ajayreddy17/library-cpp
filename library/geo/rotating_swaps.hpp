#ifndef AJAY_ROTATING_SWAPS
#define AJAY_ROTATING_SWAPS

#include "library/geo/angle_sort.hpp"

namespace mitsuha{
/*
return : {init, events}
init: order when sorted by direction vector (-1,-eps)
events: sequence of events where ranks of i,j swap (from i<j to j<i)
Covers full 360 degrees, returns to init at the end
https://atcoder.jp/contests/tupc2023/tasks/tupc2023_m
https://atcoder.jp/contests/pakencamp-2025-day2/tasks/pakencamp_2025_day2_f
*/
template <typename T>
pair<vector<int>, vector<pair<int, int>>> rotating_swaps(vector<Point<T>> point) {
  using P = Point<T>;
  /*
  On tie-breaking at angle t:
  Let p1,...,pk be the sorted order at angle t-eps
  These all tie simultaneously and then get reversed
  Swap (i,j) in pair-lexicographic order

  On points with identical coordinates:
  Points with same coordinates are always in original index order
  (..i1,...,i2) (...,j1,...,j2)
  j prioritizes smaller values, i prioritizes larger values
  */
  for (auto &p : point) p = -p;
  auto I = argsort(point);
  for (auto &p : point) p = -p;
  point = rearrange(point, I);
  int N = len(point);
  vector<pair<int, int>> pairs;
  pairs.reserve(N * (N - 1));

  For(i, N) For(j, N) {
    if (point[i] != point[j]) pairs.emplace_back(i, j);
  }

  sort(All(pairs), [&](auto &L, auto &R) -> bool {
    if (L == R) return 0;
    auto [i1, j1] = L;
    auto [i2, j2] = R;
    P p1 = (point[j1] - point[i1]).rot90(true);
    P p2 = (point[j2] - point[i2]).rot90(true);
    int c = angle_comp_3(p1, p2);
    if (c != 0) return c == -1;
    // p1,p2 are parallel
    if (point[i1].dot(p1) != point[i2].dot(p1)) {
      return point[i1].dot(p1) < point[i2].dot(p1);
    }
    // compare by lexicographic order of dot pairs at p1 - eps
    P d = p1.rot90(false);
    pair<T, T> s1 = {point[j1].dot(d), point[i1].dot(d)};
    pair<T, T> s2 = {point[j2].dot(d), point[i2].dot(d)};
    if (s1 != s2) return s1 < s2;
    // at this point the coordinate pairs are exactly identical
    assert(point[i1] == point[i2] && point[j1] == point[j2]);
    if (i1 != i2) return i1 > i2;
    return j1 < j2;
  });
  for (auto &[i, j] : pairs) {
    i = I[i], j = I[j];
  }
  return {I, pairs};
}

} // namespace mitsuha
#endif // AJAY_ROTATING_SWAPS
