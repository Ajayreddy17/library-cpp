#ifndef AJAY_SPS_COMPOSITION
#define AJAY_SPS_COMPOSITION

#include "library/setfunc/ranked_zeta.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/setfunc/sps_composition.hpp

// sum_i f_i/i! s^i, s^i is subset-convolution
template <typename mint, int LIM>
vector<mint> sps_composition_egf(vector<mint> f, vector<mint>& s) {
  const int N = topbit(len(s));
  assert(len(s) == (1 << N) && s[0] == mint(0));
  if (f.empty()) return vector<mint>(1 << N, mint(0));
  if (len(f) > N) f.resize(N + 1);
  int D = len(f) - 1;
  using ARR = array<mint, LIM + 1>;
  vector<vector<ARR>> zs(N);
  For(i, N) {
    zs[i] =
        ranked_zeta<mint, LIM>({s.begin() + (1 << i), s.begin() + (2 << i)});
  }

  // dp : (d/dt)^df(s) (d=D,D-1,...)
  vector<mint> dp(1 << (N - D));
  dp[0] = f[D];
  Frr(d, D) {
    vector<mint> newdp(1 << (N - d));
    newdp[0] = f[d];
    vector<ARR> zdp = ranked_zeta<mint, LIM>(dp);
    For(i, N - d) {
      // zs[1<<i:2<<i], zdp[0:1<<i]
      vector<ARR> znewdp(1 << i);
      For(k, 1 << i) {
        For(p, i + 1) For(q, i - p + 1) {
          znewdp[k][p + q] += zdp[k][p] * zs[i][k][q];
        }
      }
      auto x = ranked_mobius<mint, LIM>(znewdp);
      copy(All(x), newdp.begin() + (1 << i));
    }
    swap(dp, newdp);
  }
  return dp;
}

// sum_i f_i s^i, s^i is subset-convolution
template <typename mint, int LIM>
vector<mint> sps_composition_poly(vector<mint> f, vector<mint> s) {
  const int N = topbit(len(s));
  assert(len(s) == (1 << N));
  if (f.empty()) return vector<mint>(1 << N, mint(0));
  // convert to egf problem
  int D = min<int>(len(f) - 1, N);
  vector<mint> g(D + 1);
  mint c = s[0];
  s[0] = 0;
  // (x+c)^i
  vector<mint> pow(D + 1);
  pow[0] = 1;
  For(i, len(f)) {
    For(j, D + 1) g[j] += f[i] * pow[j];
    Frr(j, D + 1) pow[j] = pow[j] * c + (j == 0 ? mint(0) : pow[j - 1]);
  }
  // to egf
  mint factorial = 1;
  For(j, D + 1) g[j] *= factorial, factorial *= mint(j + 1);
  return sps_composition_egf<mint, LIM>(g, s);
}
} // namespace mitsuha
#endif // AJAY_SPS_COMPOSITION
