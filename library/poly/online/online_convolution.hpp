#ifndef AJAY_ONLINE_CONVOLUTION
#define AJAY_ONLINE_CONVOLUTION

#include "library/poly/ntt.hpp"
#include "library/mod/modint.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/online/online_convolution.hpp

/*
query(i): given a[i], b[i], returns ab[i].
2^{17}: 127ms
2^{18}: 277ms
2^{19}: 570ms
2^{20}: 1220ms
*/
template <class mint>
struct Online_Convolution {
  vector<mint> f, g, h, b0, b1;
  vector<vector<mint>> fm, gm;
  int p;

  Online_Convolution() : p(0) { assert(mint::can_ntt()); }

  mint query(int i, mint f_i, mint g_i) {
    assert(i == p);
    f.emplace_back(f_i), g.emplace_back(g_i);
    int z = __builtin_ctz(p + 2), w = 1 << z, s;
    if (p + 2 == w) {
      b0 = f, b0.resize(2 * w);
      ntt(b0, false);
      fm.emplace_back(b0.begin(), b0.begin() + w);
      b1 = g, b1.resize(2 * w);
      ntt(b1, false);
      gm.emplace_back(b1.begin(), b1.begin() + w);
      For(i, 2 * w) b0[i] *= b1[i];
      s = w - 2;
      h.resize(2 * s + 2);
    } else {
      b0.assign(f.end() - w, f.end()), b0.resize(2 * w);
      ntt(b0, false);
      For(i, 2 * w) b0[i] *= gm[z][i];
      b1.assign(g.end() - w, g.end()), b1.resize(2 * w);
      ntt(b1, false);
      For(i, 2 * w) b0[i] += b1[i] * fm[z][i];
      s = w - 1;
    }
    ntt(b0, true);
    For(i, s + 1) h[p + i] += b0[s + i];
    return h[p++];
  }
};
} // namespace mitsuha
#endif // AJAY_ONLINE_CONVOLUTION
