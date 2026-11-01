#ifndef AJAY_ONLINE_EXP
#define AJAY_ONLINE_EXP

#include "library/poly/online/online_convolution.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/online/online_exp.hpp

// query(i): given f[i], returns (f^{-1})[i].
template <typename mint>
struct Online_Exp {
  vector<mint> F;
  Online_Convolution<mint> X;

  mint query(int i, mint f_i) {
    assert(i == len(F));
    if (i == 0) {
      assert(f_i == mint(0));
      F.emplace_back(mint(1));
      return mint(1);
    }
    mint x = inv<mint>(i) * X.query(i - 1, F[i - 1], f_i * mint(i));
    F.emplace_back(x);
    return x;
  }
};
} // namespace mitsuha
#endif // AJAY_ONLINE_EXP
