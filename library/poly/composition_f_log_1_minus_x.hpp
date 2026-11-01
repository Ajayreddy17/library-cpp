#ifndef AJAY_COMPOSITION_F_LOG_1_MINUS_X
#define AJAY_COMPOSITION_F_LOG_1_MINUS_X

#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/partial_frac_decomposition_1.hpp"
#include "library/seq/famous/stirling_number_1.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/composition_f_log_1_minus_x.hpp


/*
f(log(1-x))
2024/05/01: noshi composition is a bit faster, so this is unused,
but it may become useful with a faster multipoint evaluation
*/
template <typename mint>
vector<mint> composition_f_log_1_minus_x(vector<mint> f) {
  int N = len(f) - 1;
  For(i, N + 1) f[i] *= fact<mint>(i);
  vector<mint> S = stirling_number_1_n<mint>(N + 1, true);
  reverse(All(S));
  f = convolution<mint>(f, S);
  f.resize(N + 1);
  vector<mint> A(N + 1);
  For(i, N + 1) A[i] = mint::raw(i);
  f = partial_frac_decomposition_1(f, A);
  For(i, len(f)) if (i & 1) f[i] = -f[i];
  f = poly_taylor_shift<mint>(f, -1);
  return f;
}
} // namespace mitsuha
#endif // AJAY_COMPOSITION_F_LOG_1_MINUS_X
