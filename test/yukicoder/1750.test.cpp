#define PROBLEM "https://yukicoder.me/problems/no/1750"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/linear_algebra/matrix_pow.hpp"

using mint = modint998;

void solve() {
  LL(N, M, T);
  vector<vector<mint>> A(N, vector<mint>(N));
  For(M) {
    LL(a, b);
    A[a][b] = 1;
    A[b][a] = 1;
  }
  A = matrix_pow(A, T);
  print(A[0][0]);
}

signed main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
