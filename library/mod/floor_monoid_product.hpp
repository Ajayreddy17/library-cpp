#ifndef AJAY_FLOOR_MONOID_PRODUCT
#define AJAY_FLOOR_MONOID_PRODUCT

#include "library/monoid/monoid_pow.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/floor_monoid_product.hpp
// https://yukicoder.me/submissions/883884
// https://qoj.ac/contest/1411/problem/7620
// product y..y x y..y x ... y..y (N copies of x)
// with floor((ak + b) / m) copies of y before the k-th x:
// the edge sequence of the lattice path under my <= ax + b.
// U: ax + b must not overflow in the range
template <typename Monoid, typename X, typename U>
X floor_monoid_product(X x, X y, U N, U a, U b, U m) {
    U c = (a * N + b) / m;
    X pre = Monoid::unit(), suf = Monoid::unit();
    while (1) {
        const U p = a / m, q = b / m;
        a %= m, b %= m;
        x = Monoid::op(x, monoid_pow<Monoid>(y, p));
        pre = Monoid::op(pre, monoid_pow<Monoid>(y, q));
        c -= (p * N + q);
        if (c == 0) break;
        const U d = (m * c - b - 1) / a + 1;
        suf = Monoid::op(y, Monoid::op(monoid_pow<Monoid>(x, N - d), suf));
        b = m - b - 1 + a, N = c - 1, c = d;
        swap(m, a), swap(x, y);
    }
    x = monoid_pow<Monoid>(x, N);
    return Monoid::op(Monoid::op(pre, x), suf);
}
} // namespace mitsuha
#endif // AJAY_FLOOR_MONOID_PRODUCT
