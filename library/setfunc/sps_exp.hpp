#ifndef AJAY_SPS_EXP
#define AJAY_SPS_EXP

#include "library/setfunc/subset_convolution.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/setfunc/sps_exp.hpp


// sum_i 1/i! s^i, s^i is subset-convolution
template <typename mint, int LIM>
vector<mint> sps_exp(vector<mint>& s) {
  const int N = msb(len(s));
  assert(len(s) == (1 << N) && s[0] == mint(0));
  vector<mint> dp(1 << N);
  dp[0] = mint(1);
  For(i, N) {
    vector<mint> a = {s.begin() + (1 << i), s.begin() + (2 << i)};
    vector<mint> b = {dp.begin(), dp.begin() + (1 << i)};
    a = subset_convolution<mint, LIM>(a, b);
    copy(a.begin(), a.end(), dp.begin() + (1 << i));
  }
  return dp;
}
} // namespace mitsuha
#endif // AJAY_SPS_EXP
