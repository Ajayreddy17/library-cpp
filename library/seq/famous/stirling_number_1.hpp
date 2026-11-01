#ifndef AJAY_STIRLING_NUMBER_1
#define AJAY_STIRLING_NUMBER_1

#include "library/poly/fps_div.hpp"
#include "library/poly/fps_pow.hpp"
#include "library/poly/poly_taylor_shift.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/seq/famous/stirling_number_1.hpp

template <typename mint>
vector<vector<mint>> stirling_number_1_2d(int nmax, int kmax, bool sgn = false) {
  vector<vector<mint>> A(nmax + 1, vector<mint>(kmax + 1));
  A[0][0] = 1;
  for (int i = 1; i <= nmax; ++i) {
    for (int j = 0; j < i + 1; ++j) {
      if (j > kmax) break;
      mint &x = A[i][j];
      if (j) x += A[i - 1][j - 1];
      x -= A[i - 1][j] * mint(i - 1);
    }
  }
  if (!sgn) {
    For(n, nmax + 1) For(i, n + 1) {
      if (i > kmax) break;
      if ((n + i) % 2 == 1) A[n][i] = -A[n][i];
    }
  }
  return A;
}

// coefficients c(n, k) of x(x+1)...(x+n-1)
// signed=true: falling factorial x(x-1)...(x-n+1)
// number of permutations of [n] with exactly k cycles
// all k for fixed n in O(n log n)
template <typename mint>
vector<mint> stirling_number_1_n(int n, bool sgn = false) {
  auto dfs = [&](auto self, int n) -> vector<mint> {
    if (n == 0) return {1};
    if (n == 1) return {0, 1};
    auto f = self(self, n / 2);
    auto g = poly_taylor_shift(f, mint(n / 2));
    f = convolution(f, g);
    if (n & 1) {
      g = {(n - 1), 1};
      f = convolution(f, g);
    }
    return f;
  };
  auto f = dfs(dfs, n);
  if (sgn) {
    For(i, n + 1) if ((n + i) % 2 == 1) f[i] = -f[i];
  }
  return f;
}

// c(n, k) for all n with k fixed
template <typename mint>
vector<mint> stirling_number_1_k(int k, int n_max, bool sgn = false) {
  if (n_max < k) {
    vector<mint> f(n_max + 1);
    return f;
  }
  int LIM = n_max - k;
  vector<mint> f(LIM + 1);
  For(i, LIM + 1) f[i] = inv<mint>(i + 1);
  f = fps_pow(f, k);
  if (sgn) {
    For(i, LIM + 1) if (i % 2 == 1) f[i] = -f[i];
  }

  mint cf = fact_inv<mint>(k);
  vector<mint> res(n_max + 1);
  For(i, len(f)) res[k + i] = cf * f[i] * fact<mint>(k + i);

  return res;
}

// s(n,i) in reverse order
// (1+0x)(1+1x)(1+2x)...(1+(N-1)x) up to [x^K]
template <typename mint>
vector<mint> stirling_number_1_suffix(long long N, long long K) {
  // first e^{Nx}-1 / e^x-1 up to [x^K]
  vector<mint> num(K + 1), den(K + 1);
  mint powN = 1;
  For(k, K + 1) {
    powN *= N;
    num[k] = fact_inv<mint>(k + 1) * powN;
    den[k] = fact_inv<mint>(k + 1);
  }
  vector<mint> S = fps_div<mint>(num, den);
  For(i, K + 1) S[i] *= fact<mint>(i);
  vector<mint> LOG_F(K + 1);
  For(i, 1, K + 1) LOG_F[i] = S[i] * inv<mint>(i) * (2 * (i & 1) - 1);
  return fps_exp(LOG_F);
}
} // namespace mitsuha
#endif // AJAY_STIRLING_NUMBER_1
