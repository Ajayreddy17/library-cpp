#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/geo/manhattan_nns.hpp"

void test() {
  For(n, 1, 20) For(m, 1, 20) {
    vector<pair<int, int>> A, B;
    For(n) {
      int a = RNG(0, 100);
      int b = RNG(0, 100);
      A.emplace_back(a, b);
    }
    For(m) {
      int a = RNG(0, 100);
      int b = RNG(0, 100);
      B.emplace_back(a, b);
    }
    auto [dist, J] = manhattan_nns(A, B);
    For(i, n) {
      For(j, m) {
        auto [x1, y1] = A[i];
        auto [x2, y2] = B[j];
        int d = abs(x1 - x2) + abs(y1 - y2);
        assert(dist[i] <= d);
        assert(j != J[i] || dist[i] == d);
      }
    }
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  test();
  solve();
  return 0;
}
