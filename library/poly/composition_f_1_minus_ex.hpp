#ifndef AJAY_COMPOSITION_F_1_MINUS_EX
#define AJAY_COMPOSITION_F_1_MINUS_EX

#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/composition_f_ex.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/composition_f_1_minus_ex.hpp


// f(1-e^x)
template <typename mint>
vector<mint> composition_1_minus_ex(vector<mint> f) {
  f = poly_taylor_shift<mint>(f, 1);
  For(i, len(f)) if (i & 1) f[i] = -f[i];
  return composition_f_ex<mint>(f);
}
} // namespace mitsuha
#endif // AJAY_COMPOSITION_F_1_MINUS_EX
