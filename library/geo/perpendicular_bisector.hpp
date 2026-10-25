#ifndef AJAY_PERPENDICULAR_BISECTOR
#define AJAY_PERPENDICULAR_BISECTOR

#include "library/geo/base.hpp"

namespace mitsuha{
template <typename T>
Line<T> perpendicular_bisector(Point<T> A, Point<T> B) {
  assert(A != B);
  T a = 2 * (B.x - A.x);
  T b = 2 * (B.y - A.y);
  T c = (A.x * A.x + A.y * A.y) - (B.x * B.x + B.y * B.y);
  return Line<T>(a, b, c);
}

} // namespace mitsuha
#endif // AJAY_PERPENDICULAR_BISECTOR
