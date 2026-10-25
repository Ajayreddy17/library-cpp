#ifndef AJAY_REFLECTION
#define AJAY_REFLECTION


#include "library/geo/base.hpp"

namespace mitsuha{
template <typename REAL, typename T, typename U>
Point<REAL> reflection(Point<T> P, Line<U> L) {
  REAL t = REAL(L.eval(P)) / (L.a * L.a + L.b * L.b);
  REAL x = P.x - 2 * t * L.a;
  REAL y = P.y - 2 * t * L.b;
  return Point<REAL>(x, y);
};

} // namespace mitsuha
#endif // AJAY_REFLECTION
