#ifndef AJAY_CONVEX_POLYGON_UNION_AREA
#define AJAY_CONVEX_POLYGON_UNION_AREA

#include "library/geo/base.hpp"

// Each is a strict convex polygon in ccw order
// O(N^2logN)
// 784 hexagons: 840ms
// https://ac.nowcoder.com/acm/contest/133876/K
namespace mitsuha{
template <typename Re>
Re convex_polygon_union_area(vector<vector<Point<long long>>> dat) {
  using P = Point<long long>;
  /*
  Decompose into segment contributions.
  Add the parts that appear on the boundary.

  For each edge, subtract the parts inside other polygons.

  Handle tie-breaking carefully.
  Same edge: prioritize the edge with the smaller index.
  */

  // strict ?
  for (auto &X : dat) {
    int n = len(X);
    if (n <= 2) continue;
    For(i, n) {
      P A = X[i], B = X[(i + 1) % n], C = X[(i + 2) % n];
      assert(ccw(A, B, C) == 1);
    }
  }

  long long N = len(dat);
  Re ANS = 0.0;
  For(i, N) {
    For(k, len(dat[i])) {
      P A = dat[i][k];
      P B = dat[i][(k + 1) % len(dat[i])];
      vector<pair<Re, Re>> ng;

      For(j, N) {
        if (j == i) continue;
        Re L = 0.0, R = 1.0;
        For(kk, len(dat[j])) {
          P C = dat[j][kk];
          P D = dat[j][(kk + 1) % len(dat[j])];

          P norm = (D - C).rot90(true);
          long long d = norm.dot(C);
          // left of CD: norm v > d

          if ((A - B).det(C - D) == 0) {
            // Parallel
            long long sgn = A.dot(norm) - d;
            if (sgn > 0) {
              continue;
            }
            elif (sgn == 0) {
              if ((B - A).dot(D - C) < 0) {
                // Opposite direction
                continue;
              }
              // Same direction
              // Remove the part coming from the smaller index
              if (j < i) {
                Re c = Re((C - A).dot(B - A)) / Re((B - A).dot(B - A));
                Re d = Re((D - A).dot(B - A)) / Re((B - A).dot(B - A));
                chmax(L, c), chmin(R, d);
                // [c,d)]
              } else {
                // Do not remove anything
                L = 1.0, R = 0.0;
              }
            }
            elif (sgn < 0) {
              // Cannot be interior
              L = 1.0, R = 0.0;
              break;
            }
            else {
              assert(0);
            }
          } else {
            // Not parallel
            // The part where the value is >= 0 gets clipped
            long long a = A.dot(norm) - d;
            long long b = B.dot(norm) - d;
            Re t = Re(0 - a) / Re(b - a);
            if (a < b) {
              chmax(L, t);
            }
            elif (a > b) { chmin(R, t); }
            else {
              assert(0);
            }
          }
        }
        if (L < R) ng.emplace_back(L, R);
      }
      vector<pair<Re, int>> event;
      for (auto &[a, b] : ng) {
        if (a >= b) continue;
        event.emplace_back(a, 1);
        event.emplace_back(b, -1);
      }

      Re prv = 0.0;
      int cnt = 0;
      sort(All(event));
      Re ans = 0.0;
      for (auto &[x, t] : event) {
        Re dx = x - prv;
        prv = x;
        if (cnt == 0) ans += dx;
        cnt += t;
      }
      Re dx = 1.0 - prv;
      ans += dx;
      long long det = A.det(B);
      ANS += det * ans;
    }
  }
  ANS /= 2;
  return ANS;
}

} // namespace mitsuha
#endif // AJAY_CONVEX_POLYGON_UNION_AREA
