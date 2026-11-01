#define PROBLEM "https://yukicoder.me/problems/no/2003"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_2d_dense.hpp"

using mint = modint998;

void solve() {
  LL(H, W, K);
  vector<string> G(H); read(G);
  vector<vector<mint>> dp(H, vector<mint>(W));
  FenwickTree_2D_Dense<Monoid_Add<mint>> bit(H, W);

  dp[0][0] = 1;
  bit.add(0, 0, 1);

  auto get_range = [&](int s) -> pair<int, int> {
    // x+y=s
    // 0<=x<=H-1
    // 0<=s-x<=W-1
    int lo = max<int>(0, s - W + 1);
    int hi = min<int>(H - 1, s);
    return {lo, hi + 1};
  };

  For(sm, 1, H + W - 1) {
    {
      auto [a, b] = get_range(sm - K - 1);
      For(x, a, b) {
        int y = sm - K - 1 - x;
        if (G[x][y] == '#') continue;
        bit.add(x, y, -dp[x][y]);
      }
    }
    {
      auto [a, b] = get_range(sm);
      For(x, a, b) {
        int y = sm - x;
        if (G[x][y] == '#') continue;
        dp[x][y] = bit.prefix_sum(x + 1, y + 1);
        bit.add(x, y, dp[x][y]);
      }
    }
  }

  print(dp[H - 1][W - 1]);
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
