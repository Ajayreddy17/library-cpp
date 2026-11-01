#define PROBLEM "https://judge.yosupo.jp/problem/subset_convolution"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/setfunc/subset_convolution.hpp"

using mint = modint998;

void solve() {
  LL(N);
  vector<mint> A(1 << N); read(A);
  vector<mint> B(1 << N); read(B);
  auto C = subset_convolution(A, B);
  print(C);
}

signed main() {
  solve();

  return 0;
}
