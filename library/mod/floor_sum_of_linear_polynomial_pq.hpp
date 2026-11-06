#ifndef AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL_PQ
#define AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL_PQ

#include "library/mod/floor_monoid_product.hpp"
#include "library/monoid/monoid_for_floor_sum_pq.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/floor_sum_of_linear_polynomial_pq.hpp
// all arguments nonnegative; T: answer type, U: ax + b must not overflow
template <typename T, int K1, int K2, typename U>
array<array<T, K2 + 1>, K1 + 1> floor_sum_of_linear_polynomial_nonnegative_pq(T p, T q, U N, U a, U b, U mod) {
    static_assert(is_same_v<U, unsigned long long> || is_same_v<U, unsigned __int128>);
    assert(a == 0 || N < (U(-1) - b) / a);
    using Mono = Monoid_for_floor_sum_pq<T, K1, K2>;
    Mono::set_pq(p, q);
    auto x = floor_monoid_product<Mono>(Mono::to_x(), Mono::to_y(), N, a, b, mod);
    return x.dp;
}

// ANS[i][j] = sum_{L <= x < R} p^x q^y x^i y^j, y = floor((ax + b) / mod)
// ax + b must not overflow in I; p must be invertible when a < 0
template <typename T, int K1, int K2, typename I>
array<array<T, K2 + 1>, K1 + 1> floor_sum_of_linear_polynomial_pq(T p, T q, I L, I R, I a, I b, I mod) {
    static_assert(is_same_v<I, long long> || is_same_v<I, __int128>);
    assert(L <= R && mod > 0);
    using Mono = Monoid_for_floor_sum_pq<T, K1, K2>;
    if (a < 0) {
        auto ANS = floor_sum_of_linear_polynomial_pq<T, K1, K2, I>(p.inverse(), q, -R + 1, -L + 1, -a, b, mod);
        For(i, K1 + 1) {
            if (i % 2 == 1) { For(j, K2 + 1) ANS[i][j] = -ANS[i][j]; }
        }
        return ANS;
    }
    Mono::set_pq(p, q);
    I ADD_X = L;
    I N = R - L;
    b += a * L;
    I ADD_Y = fld<I>(b, mod);
    b -= ADD_Y * mod;
    assert(a >= 0 && b >= 0);

    using Data = typename Mono::Data;
    using U = conditional_t<is_same_v<I, long long>, unsigned long long, unsigned __int128>;
    Data A = floor_monoid_product<Mono, Data, U>(Mono::to_x(), Mono::to_y(), N, a, b, mod);
    Data offset = Mono::unit();
    offset.dx = T(ADD_X), offset.dy = T(ADD_Y);
    A = Mono::op(offset, A);
    T mul = p.pow(ADD_X) * q.pow(ADD_Y);
    For(i, K1 + 1) For(j, K2 + 1) A.dp[i][j] *= mul;
    return A.dp;
}
} // namespace mitsuha
#endif // AJAY_FLOOR_SUM_OF_LINEAR_POLYNOMIAL_PQ
