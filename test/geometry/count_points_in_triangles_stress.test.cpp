#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/geo/count_points_in_triangles.hpp"
#include "library/random/base.hpp"

void test() {
  using P = Point<long long>;
  vector<P> A, B;
  int K = 1'000'000'000;
  int N = 100, M = 100;
  For(N) {
    bool small = RNG(0, 2);
    if (small) {
      A.emplace_back(RNG(-4, 5), RNG(-4, 5));
    } else {
      A.emplace_back(RNG(-K, K), RNG(-K, K));
    }
  }
  For(M) {
    bool small = RNG(0, 2);
    if (small) {
      B.emplace_back(RNG(-4, 5), RNG(-4, 5));
    } else {
      B.emplace_back(RNG(-K, K), RNG(-K, K));
    }
  }
  Count_Points_in_Triangles X(A, B);
  For(100) {
    int i = RNG(0, N), j = RNG(0, N), k = RNG(0, N);
    int me = X.count3(i, j, k);
    int naive = 0;
    for (auto&& p : B) {
      long long s1 = (A[j] - A[i]).det(p - A[i]);
      long long s2 = (A[k] - A[j]).det(p - A[j]);
      long long s3 = (A[i] - A[k]).det(p - A[k]);
      long long s = (A[k] - A[i]).det(A[j] - A[i]);
      if (s1 == 0 || s2 == 0 || s3 == 0 || s == 0) continue;
      if (abs(s1) + abs(s2) + abs(s3) == abs(s)) ++naive;
    }
    assert(me == naive);
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  For(100) test();
  solve();
  return 0;
}
