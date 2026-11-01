#define PROBLEM "https://yukicoder.me/problems/no/1618"
#include "library/template.hpp"
#include "library/poly/convolution.hpp"

void solve() {
  LL(N);
  vector<long long> A(N); read(A);
  vector<long long> B(N); read(B);
  A.insert(A.begin(), 0);
  B.insert(B.begin(), 0);
  vector<long long> f(N + 1);
  iota(f.begin(), f.end(), 0);
  A = convolution(A, f);
  B = convolution(B, f);
  For(i, len(A)) A[i] += B[i];
  A.erase(A.begin());
  print(A);
}

signed main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << setprecision(15);

  long long T = 1;
  // LL(T);
  For(_, T) solve();

  return 0;
}
