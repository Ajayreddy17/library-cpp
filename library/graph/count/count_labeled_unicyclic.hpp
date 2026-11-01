#ifndef AJAY_COUNT_LABELED_UNICYCLIC
#define AJAY_COUNT_LABELED_UNICYCLIC

#include "library/poly/fps_log.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_labeled_unicyclic.hpp

// https://oeis.org/A057500
template <typename mint>
vector<mint> count_labeled_unicyclic(long long nmax) {
  // cycle of length k: sequences of k rooted trees, divided by (2k)
  // this is sum f^{k} / (2k), so it can be counted with log
  vector<mint> f(nmax + 1);
  For(n, 1, nmax) {
    f[n] = mint(n).pow(n - 1) * fact_inv<mint>(n); // rooted
  }
  vector<mint> F(nmax + 1);
  F[0] = 1;
  For(i, 1, nmax + 1) F[i] = -f[i];
  F = fps_log(F);
  For(i, nmax + 1) F[i] = -F[i];
  // k = 1
  For(i, nmax + 1) F[i] -= f[i];
  // k = 2
  f = convolution(f, f);
  For(i, nmax + 1) F[i] -= f[i] * inv<mint>(2);
  For(i, nmax + 1) F[i] *= fact<mint>(i) * inv<mint>(2);
  return F;
}
} // namespace mitsuha
#endif // AJAY_COUNT_LABELED_UNICYCLIC
