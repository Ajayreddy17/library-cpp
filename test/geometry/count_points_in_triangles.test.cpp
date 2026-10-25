#define PROBLEM "https://judge.yosupo.jp/problem/count_points_in_triangle"
#include "library/template.hpp"

#include "library/geo/count_points_in_triangles.hpp"

using P = Point<long long>;

void solve() {
  LL(N);
  vector<P> A(N);
  For(i, N) read(A[i]);
  LL(M);
  vector<P> B(M);
  For(i, M) read(B[i]);

  Count_Points_in_Triangles X(A, B);

  LL(Q);
  For(Q) {
    INT(a, b, c);
    print(X.count3(a, b, c));
  }
}

signed main() {
  solve();
  return 0;
}
