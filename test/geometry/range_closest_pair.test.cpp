#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/geo/range_closest_pair_query.hpp"

void test() {
  For(N, 2, 100) {
    For(Q, 1, 100) {
      vector<pair<int, int>> point(N), query(Q);
      For(i, N) {
        int x = RNG(0, 20);
        int y = RNG(0, 20);
        point[i] = {x, y};
      }
      For(q, Q) {
        while (1) {
          int L = RNG(0, N);
          int R = RNG(0, N);
          if (L + 1 <= R) {
            query[q] = {L, R + 1};
            break;
          }
        }
      }
      Range_Closest_Pair_Query X;
      for (auto&& [a, b]: point) X.add_point(a, b);
      for (auto&& [l, r]: query) X.add_query(l, r);
      vector<long long> ANS = X.calc();
      For(q, Q) {
        long long ans = linf;
        auto [L, R] = query[q];
        For(i, L, R) For(j, L, R) {
          if (i == j) continue;
          auto [x1, y1] = point[i];
          auto [x2, y2] = point[j];
          long long dx = x1 - x2, dy = y1 - y2;
          chmin(ans, dx * dx + dy * dy);
        }
        assert(ans == ANS[q]);
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
