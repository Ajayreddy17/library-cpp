#define PROBLEM "https://yukicoder.me/problems/no/1667"
#include "library/template.hpp"
#include "library/mod/dynamic_modint.hpp"

using mint = dmint;

void solve() {
  LL(N, mod);
  mint::set_mod(mod);

  vector<mint> f(N + 1);
  For(n, 1, N + 1) f[n] = (n == 1 ? mint(1) : mint(n).pow(n - 2));

  // 点、辺
  vector<vector<mint>> DP(N + 1, vector<mint>(N + 1));
  DP[0][0] = 1;
  For(n, 1, N + 1) {
    For(m, n + 1) {
      For(k, 1, n + 1) {
        mint cf = C<mint, false, true>(n - 1, k - 1);
        cf *= f[k];
        long long n1 = n - k;
        long long m1 = m - (k - 1);
        if (n1 < 0 || m1 < 0) continue;
        DP[n][m] += DP[n1][m1] * cf;
      }
    }
  }
  vector<mint> ANS = DP[N];
  ANS.pop_back();
  for (auto&& x: ANS) print(x);
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
