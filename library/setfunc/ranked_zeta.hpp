#ifndef AJAY_RANKED_ZETA
#define AJAY_RANKED_ZETA

#include "library/setfunc/bitwise_transform.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/setfunc/ranked_zeta.hpp


template <typename T, int LIM>
vector<array<T, LIM + 1>> ranked_zeta(const vector<T>& f) {
  int n = topbit(len(f));
  assert(n <= LIM);
  assert(len(f) == 1 << n);
  vector<array<T, LIM + 1>> Rf(1 << n);
  for (int s = 0; s < (1 << n); ++s) Rf[s][popcnt(s)] = f[s];
  bitwise::bitwise_transform<bitwise::trans_type::ranked_zeta>(Rf);
  return Rf;
}

template <typename T, int LIM>
vector<T> ranked_mobius(vector<array<T, LIM + 1>>& Rf) {
  bitwise::bitwise_transform<bitwise::trans_type::ranked_mobius>(Rf);
  vector<T> f(len(Rf));
  for (int s = 0; s < len(f); ++s) f[s] = Rf[s][popcnt(s)];
  return f;
}
} // namespace mitsuha
#endif // AJAY_RANKED_ZETA
