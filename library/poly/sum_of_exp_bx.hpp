#ifndef AJAY_SUM_OF_EXP_BX
#define AJAY_SUM_OF_EXP_BX

#include "library/poly/sum_of_rationals.hpp"
#include "library/poly/fps_inv.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/sum_of_exp_bx.hpp

// sum a e^{bx} up to degree N. O(Mlog^2M + NlogN)
template <typename mint>
vector<mint> sum_of_exp_bx(int N, vector<mint> A, vector<mint> B) {
  auto [f, g] = sum_of_rationals_1<mint>(B, A);
  g.resize(N + 1);
  f = convolution(f, fps_inv(g));
  f.resize(N + 1);
  For(n, N + 1) f[n] *= fact_inv<mint>(n);
  return f;
}
} // namespace mitsuha
#endif // AJAY_SUM_OF_EXP_BX
