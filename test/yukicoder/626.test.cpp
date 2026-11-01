#define PROBLEM "https://yukicoder.me/problems/no/626"
#include "library/template.hpp"
#include "library/knapsack/knapsack_branch_bound.hpp"

void solve() {
  LL(N, LIM);
  vector<long long> wt(N), val(N);
  For(i, N) {
    LL(a, b);
    val[i] = a, wt[i] = b;
  }

  long long ANS = knapsack_branch_bound(wt, val, LIM);
  print(ANS);
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
