#ifndef AJAY_TRIANGLE_AREA
#define AJAY_TRIANGLE_AREA

#include "library/geo/base.hpp"

namespace mitsuha{
template <typename REAL, typename T>
REAL triangle_area(Point<T> A, Point<T> B, Point<T> C) {
    return abs((B - A).det(C - A)) * 0.5;
}
} // namespace mitsuha
#endif // AJAY_TRIANGLE_AREA