#ifndef AJAY_ANNULUS_WALK_GENERATING_FUNCTION
#define AJAY_ANNULUS_WALK_GENERATING_FUNCTION

#include "library/poly/poly_taylor_shift.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/annulus_walk_generating_function.hpp

// return: P,Q such that (# of walk s->t of step K)=[x^K]P(x)/Q(x)
// stay==0: O(N)time
// stay!=0: O(NlogN)time
template <typename mint>
pair<vector<mint>, vector<mint>> annulus_walk_generating_function(
    int N, int s, int t, mint stay) {
  int k = bmod<int>(t - s, N);
  auto get_D = [&](int m) -> vector<mint> {
    if (m < 0) return vector<mint>{mint(0)};
    vector<mint> f(m + 1);
    for (int j = 0; 2 * j <= m; ++j) {
      f[2 * j] = (j & 1 ? -mint(1) : mint(1)) * C<mint>(m - j, j);
    }
    return f;
  };

  vector<mint> P(N), Q(N + 1);

  auto add = [&](vector<mint>& f, const vector<mint>& g, int shift, mint coef) {
    For(i, len(g)) {
      if (i + shift >= len(f)) break;
      f[i + shift] += coef * g[i];
    }
  };

  add(P, get_D(N - k - 1), k, 1);
  add(P, get_D(k - 1), N - k, 1);
  add(Q, get_D(N), 0, 1);
  add(Q, get_D(N - 2), 2, -1);
  Q[N] -= 2;
  auto conv = [&](vector<mint>& f) -> void {
    reverse(All(f));
    f = poly_taylor_shift<mint>(f, -stay);
    reverse(All(f));
  };
  if (stay != 0) {
    conv(P), conv(Q);
  }
  return {P, Q};
}
} // namespace mitsuha
#endif // AJAY_ANNULUS_WALK_GENERATING_FUNCTION
