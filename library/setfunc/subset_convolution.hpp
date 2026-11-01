#ifndef AJAY_SUBSET_CONVOLUTION
#define AJAY_SUBSET_CONVOLUTION

#include "library/setfunc/ranked_zeta.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/setfunc/subset_convolution.hpp

template <typename T, int LIM = 20>
vector<T> subset_convolution_square(const vector<T>& A) {
  auto RA = ranked_zeta<T, LIM>(A);
  int n = topbit(len(RA));
  For(s, len(RA)) {
    auto& f = RA[s];
    Frr(d, n + 1) {
      T x = 0;
      For(i, d + 1) x += f[i] * f[d - i];
      f[d] = x;
    }
  }
  return ranked_mobius<T, LIM>(RA);
}

template <typename T, int LIM = 20>
vector<T> subset_convolution(const vector<T>& A, const vector<T>& B) {
  if (A == B) return subset_convolution_square(A);
  auto RA = ranked_zeta<T, LIM>(A);
  auto RB = ranked_zeta<T, LIM>(B);
  int n = topbit(len(RA));
  For(s, len(RA)) {
    auto &f = RA[s], &g = RB[s];
    Frr(d, n + 1) {
      T x = 0;
      For(i, d + 1) x += f[i] * g[d - i];
      f[d] = x;
    }
  }
  return ranked_mobius<T, LIM>(RA);
}
} // namespace mitsuha
#endif // AJAY_SUBSET_CONVOLUTION
