#define PROBLEM "https://yukicoder.me/problems/no/2810"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/linear_algebra/matrix_pow.hpp"

using mint = modint998;
using MAT6 = array<array<mint, 6>, 6>;
using MAT = array<array<mint, 12>, 12>;

void apply6(MAT6 A, array<mint, 12> &x) {
    array<mint, 12> y;
    For(i, 6) For(j, 6) {
        y[j] += A[i][j] * x[i];
        y[6 + j] += A[i][j] * x[6 + i];
    }
    x = y;
}

void apply(MAT A, array<mint, 12> &x) {
    array<mint, 12> y;
    For(i, 12) For(j, 12) { y[j] += A[i][j] * x[i]; }
    x = y;
}

MAT6 base;
MAT X;

MAT6 POW[30];

void solve(long long N, long long M, long long C) {
    array<mint, 12> A{};
    A[0] = 1;
    For(i, 30) if (C >> i & 1) apply6(POW[i], A);
    A[6] += A[0], A[0] = 0;
    apply(X, A);
    For(i, 30) if ((N - C) >> i & 1) apply6(POW[i], A);
    mint ANS = 0;
    For(i, 6, 12) ANS += A[i];
    print(ANS);
}

void solve() {
    LL(N, M, Q);

    For(i, 1, 6) { base[i][i - 1] = 1; }
    For(j, 6) base[0][j] = 1;

    auto tmp = matrix_pow<mint, 6>(base, N);
    For(i, 6) For(j, 6) X[i][j] = X[i + 6][j + 6] = tmp[i][j];
    For(i, 12) X[i][6] += X[i][0], X[i][0] = 0;
    X = matrix_pow<mint, 12>(X, M - 1);
    POW[0] = base;
    For(i, 29) POW[i + 1] = matrix_mul<mint, 6>(POW[i], POW[i]);

    For(Q) {
        LL(c);
        solve(N, M, c);
    }
}

signed main() {
    int T = 1;
    // INT(T);
    For(T) solve();
    return 0;
}
