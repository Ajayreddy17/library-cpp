#define PROBLEM "https://yukicoder.me/problems/no/1332"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"

void solve() {
  LL(N);
  vector<long long> X(N); read(X);
  Wavelet_Matrix<long long, false> WM(X);

  LL(Q);
  For(Q) {
    LL(l, r, x);
    --l;
    long long ANS = infty<long long>;
    long long n = WM.count(l, r, 0, x);
    if (n > 0) chmin(ANS, abs(x - WM.kth(l, r, n - 1)));
    if (n < r - l) chmin(ANS, abs(x - WM.kth(l, r, n)));
    print(ANS);
  }
}

signed main() {
  solve();
  return 0;
}
