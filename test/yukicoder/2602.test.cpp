#define PROBLEM "https://yukicoder.me/problems/no/2602"
#include "library/template.hpp"
#include "library/geo/base.hpp"
#include "library/geo/outcircle.hpp"

using P = Point<long long>;

void solve() {
  LL(Q);
  P A, B, C;
  read(A, B, C);

  A.x *= 2;
  A.y *= 2;
  B.x *= 2;
  B.y *= 2;
  C.x *= 2;
  C.y *= 2;

  For(3) {
    tie(A, B, C) = make_tuple(B, C, A);
    long long dot = (B - A).dot(C - A);
    if (dot < 0) {
      P O = (B + C);
      O.x /= 2, O.y /= 2;
      A = B - O;
      tie(A.x, A.y) = make_pair(A.y, -A.x);
      A = A + O;
    }
  }

  For(Q) {
    P p;
    read(p);
    p.x *= 2, p.y *= 2;
    print(Yes[outcircle_side(A, B, C, p) >= 0]);
  }
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
