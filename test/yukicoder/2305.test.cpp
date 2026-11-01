#define PROBLEM "https://yukicoder.me/problems/no/2305"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/random/base.hpp"
#include "library/graph/base.hpp"
#include "library/seq/find_linear_rec.hpp"
#include "library/poly/coef_of_rational_fps.hpp"

using mint = modint998;

void solve() {
  LL(N);
  ++N;
  vector<int> par(N, -1);
  vector<mint> W(N, -1);
  For(i, 1, N) { read(par[i]); }
  For(i, 1, N) read(W[i]);

  Graph<mint, 0> G(N);
  For(i, 1, N) { G.add(par[i], i, W[i]); }
  G.build();
  // sparse matrix
  using T = tuple<int, int, mint>;
  vector<T> dat;
  For(v, N) {
    vector<int> TO;
    vector<mint> WT;
    for (auto&& e: G[v]) {
      if (e.to == par[v]) continue;
      TO.emplace_back(e.to), WT.emplace_back(e.cost);
    }
    if (!TO.empty()) {
      mint cf = accumulate(WT.begin(), WT.end(), (mint)0);
      cf = mint(1) / cf;
      For(i, len(TO)) dat.emplace_back(v, TO[i], WT[i] * cf);
    } else {
      dat.emplace_back(v, 0, 1);
    }
  }

  auto apply = [&](vector<mint> X) -> vector<mint> {
    vector<mint> Y(N);
    for (auto&& [a, b, c]: dat) Y[b] += X[a] * c;
    return Y;
  };

  int L = N + N + 10;
  vector<vector<mint>> dp(L, vector<mint>(N));
  dp[0][0] = mint(1);
  For(i, L - 1) dp[i + 1] = apply(dp[i]);
  For(i, L - 1) For(j, N) dp[i + 1][j] += dp[i][j];

  vector<mint> wt(N);
  For(i, N) wt[i] = RNG(0, mint::get_mod() - 1);
  vector<mint> S(L);
  For(i, L) For(j, N) S[i] += dp[i][j] * wt[j];

  using poly = vector<mint>;
  poly f = find_linear_rec(S);
  long long n = len(f);

  LL(Q);
  For(Q) {
    LL(v, K);

    vector<mint> A(n - 1);
    For(i, n - 1) A[i] = dp[i][v];

    // 数列 A, 漸化式 f を補間
    poly g = convolution(A, f);
    g.resize(n - 1);
    mint ANS = coef_of_rational_fps(g, f, K);

    if (v == 0) ANS -= mint(1);
    print(ANS);
  }
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
