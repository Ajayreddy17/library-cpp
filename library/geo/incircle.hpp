#ifndef AJAY_INCIRCLE
#define AJAY_INCIRCLE

#include "library/geo/base.hpp"
#include "library/geo/triangle_area.hpp"

namespace mitsuha{
template <typename REAL, typename T>
Circle<REAL> incircle(Point<T> A, Point<T> B, Point<T> C) {
  REAL a = distance<REAL, T, T>(B, C);
  REAL b = distance<REAL, T, T>(C, A);
  REAL c = distance<REAL, T, T>(A, B);
  REAL x = (a * A.x + b * B.x + c * C.x) / (a + b + c);
  REAL y = (a * A.y + b * B.y + c * C.y) / (a + b + c);
  REAL r = 2 * triangle_area<REAL>(A, B, C) / (a + b + c);
  return Circle<REAL>(x, y, r);
}

} // namespace mitsuha
#endif // AJAY_INCIRCLE
