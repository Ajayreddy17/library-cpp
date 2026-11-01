#define PROBLEM "https://yukicoder.me/problems/no/1818"
#include "library/template.hpp"
#include "library/string/edit_distance.hpp"

void solve() {
  LL(N, M);
  vector<long long> A(N); read(A);
  vector<long long> B(M); read(B);
  string S, T;
  For(i, N) {
    For(A[i]) S += 'a';
    S += 'b';
  }
  For(i, M) {
    For(B[i]) T += 'a';
    T += 'b';
  }

  print(edit_distance(S, T));
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
