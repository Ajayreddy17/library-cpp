#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/count/count_labeled_strong_tournament.hpp"

using mint = modint998;

void test() {
  int N = 10;
  vector<mint> F = count_labeled_strong_tournament<mint>(N);
  vector<long long> ANS = {0,
            1,
            0,
            2,
            24,
            544,
            22320,
            1677488,
            236522496,
            64026088576,
            33832910196480};
  For(n, 1, 11) assert(F[n] == mint(ANS[n]));
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
