#define PROBLEM "https://yukicoder.me/problems/no/1919"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_2d.hpp"
#include "library/monoid/monoid_add_pair.hpp"

using mint = modint107;

void solve() {
  LL(N);
  vector<int> A(N); read(A);
  vector<int> B(N); read(B);

  pair<mint, mint> ANS;
  For(2) {
    swap(ANS.first, ANS.second);
    swap(A, B);
    auto I = argsort(A);
    A = rearrange(A, I);
    B = rearrange(B, I);
    vector<int> X(N), Y(N);
    For(i, N) X[i] = A[i] - B[i];
    For(i, N) Y[i] = A[i] + B[i];

    using Grp = Monoid_Add_Pair<mint>;
    FenwickTree_2D<Grp, int, false> bit(X, Y);
    For(i, N) {
      bit.add(X[i], Y[i], {mint(1), mint(A[i])});
      auto [c, s] = bit.prefix_sum(X[i], Y[i]);
      ANS.first += mint(A[i]) * c - s;
    }
  }
  ANS.first *= mint(2);
  ANS.second *= mint(2);
  print(ANS);
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
