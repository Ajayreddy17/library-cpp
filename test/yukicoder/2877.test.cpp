#define PROBLEM "https://yukicoder.me/problems/no/2877"
#include "library/template.hpp"
#include "library/poly/multipoint.hpp"
#include "library/mod/primitive_root.hpp"
#include "library/mod/modint.hpp"
#include "library/seq/interpolate_linear_rec.hpp"

using mint = modint998;

void solve() {
  LL(N, H);
  mint r = primitive_root(mint::get_mod());
  mint x = 1;
  vector<mint> Y(N + 1);

  For(p, N + 1) {
    vector<mint> dp = {mint(2) * inv<mint>(5), mint(2) * inv<mint>(5) * x,
                   inv<mint>(5) * x};
    vector<mint> A;
    vector<tuple<int, int, mint>> tr;
    tr.emplace_back(0, 0, inv<mint>(2));
    tr.emplace_back(0, 1, inv<mint>(2) * x);

    tr.emplace_back(1, 0, inv<mint>(3));
    tr.emplace_back(1, 1, inv<mint>(3) * x);
    tr.emplace_back(1, 2, inv<mint>(3) * x);

    tr.emplace_back(2, 1, inv<mint>(3) * mint(2) * x);
    tr.emplace_back(2, 2, inv<mint>(3) * x);
    For(7) {
      A.emplace_back(accumulate(dp.begin(), dp.end(), (mint)0));
      vector<mint> newdp(3);
      for (auto& [a, b, c] : tr) newdp[b] += dp[a] * c;
      swap(dp, newdp);
    }
    Y[p] = interpolate_linear_rec<mint>(A, N - 1, 0);
    x *= r;
  }

  vector<mint> F = multipoint_interpolate_on_geom_seq<mint>(Y, 1, r);

  mint ANS = 0;
  For(i, H, N + 1) ANS += F[i];
  // print(F);
  print(ANS);
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
