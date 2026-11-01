#define PROBLEM "https://yukicoder.me/problems/no/1810"
#include "library/template.hpp"
#include "library/linear_algebra/matrix_mul.hpp"
#include "library/linear_algebra/matrix_pow.hpp"
#include "library/mod/modint.hpp"

using mint = modint107;

void solve() {
  mint A, B;
  read(A), read(B);
  vector<vector<mint>> P(3, vector<mint>(3));
  vector<vector<mint>> Q(3, vector<mint>(3));
  P[0] = {1, 0, 0};
  P[1] = {0, 1, 0};
  P[2] = {A, B, 1};
  Q[0] = {0, 0, 1};
  Q[1] = {1, 0, 0};
  Q[2] = {0, 0, 0};
  auto QP = matrix_mul(Q, P);
  LL(T);
  For(T) {
    LL(t);
    auto X = matrix_pow(QP, t / 2);
    if (t & 1) X = matrix_mul(P, X);
    mint ANS = 0;
    For(i, 3) For(j, 2) ANS += X[i][j];
    print(ANS);
  }
}

signed main() {
  solve();
  return 0;
}
