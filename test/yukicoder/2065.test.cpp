#define PROBLEM "https://yukicoder.me/problems/no/2065"
#include "library/template.hpp"
#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"
#include "library/datastructure/static_range_product_group.hpp"

void solve() {
  LL(N, Q);
  vector<long long> A(N); read(A);
  Wavelet_Matrix<long long, false, Static_Range_Product_Group<Monoid_Add<long long>>> WM(
      N, [&](int i) -> pair<int, long long> {
        return {A[i], A[i]};
      });
  For(Q) {
    LL(l, r, x);
    --l;
    auto [cnt, sm] = WM.count_and_prod(l, r, 0, x);
    long long ANS = (r - l - cnt) * x + sm;
    print(ANS);
  }
}

signed main() {
  solve();
  return 0;
}
