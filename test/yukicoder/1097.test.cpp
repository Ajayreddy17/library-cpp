#define PROBLEM "https://yukicoder.me/problems/no/1097"
#include "library/template.hpp"
#include "library/datastructure/doubling.hpp"
#include "library/monoid/monoid_add.hpp"

void solve() {
  LL(N);
  vector<long long> A(N); read(A);
  Doubling<Monoid_Add<long long>, 50> X(N);
  For(i, N) { X.add(i, (i + A[i]) % N, A[i]); }
  X.build();

  LL(Q);
  For(Q) {
    LL(k);
    auto [to, val] = X.calc(0, k);
    print(val);
  }
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
