#define PROBLEM "https://yukicoder.me/problems/no/924"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"
#include "library/datastructure/static_range_product_group.hpp"

void solve() {
  LL(N, Q);
  vector<long long> A(N); read(A);
  Wavelet_Matrix<int, true, Static_Range_Product_Group<Monoid_Add<long long>>> WM(
      N, [&](int i) -> pair<int, long long> { return {A[i], A[i]}; });
  For(Q) {
    LL(L, R);
    --L;
    int n = (R - L);
    long long k = (n - 1) / 2;
    long long med = WM.kth(L, R, k);
    long long sum_lo = WM.prefix_prod(L, R, med);
    auto sum_hi = WM.prod_all(L, R) - sum_lo;
    long long ANS = 0;
    ANS += med * k - sum_lo;
    ANS += sum_hi - med * (R - L - k);
    print(ANS);
  }
}

signed main() {
  solve();
  return 0;
}
