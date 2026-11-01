#define PROBLEM "https://yukicoder.me/problems/no/1500"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/seq/find_linear_rec.hpp"
#include "library/seq/interpolate_linear_rec.hpp"

using mint = modint107;

void solve() {
  vector<mint> A(20);
  set<pair<long long, long long>> ss;
  ss.insert(make_pair(0, 0));
  For(i, 20) {
    A[i] = len(ss);
    set<pair<long long, long long>> newss;
    for (auto&& [x, y]: ss) {
      newss.insert(make_pair(x + 3, y + 0));
      newss.insert(make_pair(x + 3, y + 2));
      newss.insert(make_pair(x + 2, y + 3));
      newss.insert(make_pair(x + 0, y + 3));
      newss.insert(make_pair(x - 2, y + 3));
      newss.insert(make_pair(x - 3, y + 2));
      newss.insert(make_pair(x - 3, y + 0));
      newss.insert(make_pair(x - 3, y - 2));
      newss.insert(make_pair(x - 2, y - 3));
      newss.insert(make_pair(x - 0, y - 3));
      newss.insert(make_pair(x + 2, y - 3));
      newss.insert(make_pair(x + 3, y - 2));
    }
    swap(ss, newss);
  }
  LL(N);
  // print(find_linear_rec(A));
  // print(A);
  print(interpolate_linear_rec(A, N, 0));
}

signed main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  cout << setprecision(15);

  long long T = 1;
  // LL(T);
  For(_, T) solve();

  return 0;
}
