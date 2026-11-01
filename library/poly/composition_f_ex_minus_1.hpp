#ifndef AJAY_COMPOSITION_F_EX_MINUS_1
#define AJAY_COMPOSITION_F_EX_MINUS_1

#include "library/poly/poly_taylor_shift.hpp"
#include "library/poly/composition_f_ex.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/composition_f_ex_minus_1.hpp


// f(1-e^x)
template <typename mint>
vector<mint> composition_f_ex_minus_1(vector<mint> f) {
  f = poly_taylor_shift<mint>(f, -1);
  return composition_f_ex<mint>(f);
}
} // namespace mitsuha
#endif // AJAY_COMPOSITION_F_EX_MINUS_1
