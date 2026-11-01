#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/count/count_unlabeled_tree.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  auto A = count_unlabeled_tree<mint>(10, false);
  vector<mint> B = {mint(0), mint(1),  mint(1),  mint(1),  mint(2),  mint(3),
                mint(6), mint(11), mint(23), mint(47), mint(106)};
  assert(A == B);
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << '\n';
}

signed main() {
  test();
  solve();
  return 0;
}
