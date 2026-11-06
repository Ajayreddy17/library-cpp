#ifndef AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL
#define AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL

#include "library/mod/floor_monoid_product.hpp"
#include "library/monoid/monoid_for_floor_sum.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/floor_sum_of_linear_polynomial.hpp
// all arguments nonnegative; T: answer type, U: ax + b must not overflow
// returns sum_{0 <= x < N} x^i floor((ax + b) / mod)^j for i <= K1, j <= K2
template <typename T, int K1, int K2, typename U>
array<array<T, K2 + 1>, K1 + 1> floor_sum_of_linear_polynomial_nonnegative(U N, U a, U b, U mod) {
    static_assert(is_same_v<U, unsigned long long> || is_same_v<U, unsigned __int128>);
    assert(a == 0 || N < (U(-1) - b) / a);
    using Mono = Monoid_for_floor_sum<T, K1, K2>;
    auto x = floor_monoid_product<Mono>(Mono::to_x(), Mono::to_y(), N, a, b, mod);
    return x.dp;
}

// ANS[i][j] = sum_{L <= x < R} x^i floor((ax + b) / mod)^j
// ax + b must not overflow in I
template <typename T, int K1, int K2, typename I>
array<array<T, K2 + 1>, K1 + 1> floor_sum_of_linear_polynomial(I L, I R, I a, I b, I mod) {
    static_assert(is_same_v<I, long long> || is_same_v<I, __int128>);
    assert(L <= R && mod > 0);
    if (a < 0) {
        auto ANS = floor_sum_of_linear_polynomial<T, K1, K2, I>(-R + 1, -L + 1, -a, b, mod);
        For(i, K1 + 1) {
            if (i % 2 == 1) { For(j, K2 + 1) ANS[i][j] = -ANS[i][j]; }
        }
        return ANS;
    }
    I ADD_X = L;
    I N = R - L;
    b += a * L;
    I ADD_Y = fld<I>(b, mod);
    b -= ADD_Y * mod;
    assert(a >= 0 && b >= 0);

    using Mono = Monoid_for_floor_sum<T, K1, K2>;
    using Data = typename Mono::Data;
    using U = conditional_t<is_same_v<I, long long>, unsigned long long, unsigned __int128>;
    Data A = floor_monoid_product<Mono, Data, U>(Mono::to_x(), Mono::to_y(), N, a, b, mod);
    Data offset = Mono::unit();
    offset.dx = T(ADD_X), offset.dy = T(ADD_Y);
    A = Mono::op(offset, A);
    return A.dp;
}
} // namespace mitsuha
#endif // AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL
