#ifndef AJAY_CONVEX_LAYERS
#define AJAY_CONVEX_LAYERS

#include "library/geo/dynamic_upper_hull.hpp"

// https://codeforces.com/contest/1599/problem/D
// Output is in counter-clockwise order
namespace mitsuha{
vector<vector<int>> convex_layers(vector<Point<long long>> points) {
  int N = len(points);
  Dynamic_Upper_Hull<Point<long long>> DUH(points, 1);
  For(i, N) points[i] = -points[i];
  Dynamic_Upper_Hull<Point<long long>> DLH(points, 1);
  vector<vector<int>> ANS;
  int done = 0;
  int k = 0;
  while (done < N) {
    ++k;
    auto A = DLH.get();
    auto B = DUH.get();
    if (len(A) >= 2) {
      A.pop_back(), B.pop_back();
      A.insert(A.end(), All(B));
      reverse(All(A));
    }
    ANS.emplace_back(A);
    for (auto&& i : A) {
      ++done;
      DUH.erase(i);
      DLH.erase(i);
    }
  }
  return ANS;
}

} // namespace mitsuha
#endif // AJAY_CONVEX_LAYERS
