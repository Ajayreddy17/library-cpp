#ifndef AJAY_OUTCIRCLE
#define AJAY_OUTCIRCLE

#include "library/geo/triangle_area.hpp"

namespace mitsuha{
template <typename REAL, typename T>
Circle<REAL> outcircle(Point<T> A, Point<T> B, Point<T> C) {
    Assert(ccw(A, B, C) != 0);
    REAL b1 = B.x - A.x, b2 = B.y - A.y;
    REAL c1 = C.x - A.x, c2 = C.y - A.y;
    REAL bb = (b1 * b1 + b2 * b2) / 2;
    REAL cc = (c1 * c1 + c2 * c2) / 2;

    REAL det = b1 * c2 - b2 * c1;
    REAL x = (bb * c2 - b2 * cc) / det;
    REAL y = (b1 * cc - bb * c1) / det;
    REAL r = sqrt(x * x + y * y);
    x += A.x, y += A.y;
    return Circle<REAL>(x, y, r);
}

// p location with respect to circumcircle of ABC
// Inside: 1, Border: 0, Outside: -1
// Make sure to use large T, i.e upto 4th power
template <typename T>
int outcircle_side(Point<T> A, Point<T> B, Point<T> C, Point<T> p) {
    T d = (B - A).det(C - A);
    assert(d != 0);
    if (d < 0) swap(B, C);
    array<Point<T>, 3> pts = {A, B, C};
    array<array<T, 3>, 3> mat;
    For(i, 3) {
        T dx = pts[i].x - p.x, dy = pts[i].y - p.y;
        mat[i][0] = dx, mat[i][1] = dy, mat[i][2] = dx * dx + dy * dy;
    }
    T det = 0;
    det += mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]);
    det += mat[0][1] * (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]);
    det += mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    if (det == 0) return 0;
    return (det > 0 ? 1 : -1);
}
} // namespace mitsuha
#endif // AJAY_OUTCIRCLE