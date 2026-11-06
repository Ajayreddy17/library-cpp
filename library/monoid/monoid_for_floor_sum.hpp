#ifndef AJAY_MONOID_FOR_FLOOR_SUM
#define AJAY_MONOID_FOR_FLOOR_SUM

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/alg/monoid/monoid_for_floor_sum.hpp
// state of a floor path: dp[i][j] = sum of x^i y^j over the x-steps taken from (x, y)
// moving one step in x adds x^K1 y^K2 terms at the current point
template <typename T, int K1, int K2>
struct Monoid_for_floor_sum {
    using ARR = array<array<T, K2 + 1>, K1 + 1>;
    struct Data {
        ARR dp;
        T dx, dy;
    };

    using value_type = Data;
    using X = value_type;
    static X op(X a, X b) {
        static constexpr int n = max(K1, K2);
        static T comb[n + 1][n + 1];
        if (comb[0][0] != T(1)) {
            comb[0][0] = T(1);
            For(i, n) For(j, i + 1) { comb[i + 1][j] += comb[i][j], comb[i + 1][j + 1] += comb[i][j]; }
        }

        array<T, K1 + 1> pow_x;
        array<T, K2 + 1> pow_y;
        pow_x[0] = 1, pow_y[0] = 1;
        For(i, K1) pow_x[i + 1] = pow_x[i] * a.dx;
        For(i, K2) pow_y[i + 1] = pow_y[i] * a.dy;

        // shift b by +dy
        For(i, K1 + 1) {
            Frr(j, K2 + 1) {
                T x = b.dp[i][j];
                For(k, j + 1, K2 + 1) b.dp[i][k] += comb[k][j] * pow_y[k - j] * x;
            }
        }
        // shift b by +dx and add to a
        For(j, K2 + 1) {
            Frr(i, K1 + 1) { For(k, i, K1 + 1) a.dp[k][j] += comb[k][i] * pow_x[k - i] * b.dp[i][j]; }
        }

        a.dx += b.dx, a.dy += b.dy;
        return a;
    }

    static X to_x() {
        X x = unit();
        x.dp[0][0] = 1, x.dx = 1;
        return x;
    }
    static X to_y() {
        X x = unit();
        x.dy = 1;
        return x;
    }
    static constexpr X unit() { return {ARR{}, T(0), T(0)}; }
    static constexpr bool commute = 0;
};
} // namespace mitsuha
#endif // AJAY_MONOID_FOR_FLOOR_SUM
