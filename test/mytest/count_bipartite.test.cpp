#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/count/count_labeled_bipartite.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  vector<mint> A = count_labeled_bipartite<mint>(10, true);
  vector<mint> B = {0LL,    1LL,     1LL,       3LL,        19LL,        195LL,
                3031LL, 67263LL, 2086099LL, 89224635LL, 5254054111LL};
  assert(A == B);
  A = count_labeled_bipartite<mint>(10, false);
  B = {1LL,    1LL,      2LL,       7LL,         41LL,        376LL,
       5177LL, 103237LL, 2922446LL, 116011231LL, 6433447397LL};
  assert(A == B);
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
