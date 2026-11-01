#define PROBLEM "https://yukicoder.me/problems/no/3005"
#include "library/template.hpp"
#include "library/geo/outcircle.hpp"

using P = Point<long long>;

void solve() {
  P A, B, C, D;
  read(A, B, C, D);
  if ((A - B).det(A - C) == 0) return print(YES[!(1)]);
  int k = outcircle_side(A, B, C, D);
  print(YES[k == 0]);
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
