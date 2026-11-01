#ifndef AJAY_SUM_OF_RATIONALS
#define AJAY_SUM_OF_RATIONALS

#include "library/poly/convolution.hpp"
#include "library/poly/ntt_doubling.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/sum_of_rationals.hpp


// sum of rational functions by divide and conquer, O(Nlog^2N), N = total degree
template <typename mint>
pair<vector<mint>, vector<mint>> sum_of_rationals(vector<pair<vector<mint>, vector<mint>>> dat) {
  if (len(dat) == 0) {
    vector<mint> f = {0}, g = {1};
    return {f, g};
  }
  using P = pair<vector<mint>, vector<mint>>;
  auto add = [&](P& a, P& b) -> P {
    int na = len(a.first) - 1, da = len(a.second) - 1;
    int nb = len(b.first) - 1, db = len(b.second) - 1;
    int n = max(na + db, da + nb);
    vector<mint> num(n + 1);
    {
      auto f = convolution(a.first, b.second);
      For(i, len(f)) num[i] += f[i];
    }
    {
      auto f = convolution(a.second, b.first);
      For(i, len(f)) num[i] += f[i];
    }
    auto den = convolution(a.second, b.second);
    return {num, den};
  };

  while (len(dat) > 1) {
    int n = len(dat);
    for (int i = 1; i < n; i += 2) dat[i - 1] = add(dat[i - 1], dat[i]);
    For(i, cld(n, 2)) dat[i] = dat[2 * i];
    dat.resize(cld(n, 2));
  }
  return dat[0];
}

// sum wt[i]/(1-A[i]x)
template <typename mint>
pair<vector<mint>, vector<mint>> sum_of_rationals_1(vector<mint> A, vector<mint> wt) {
  using poly = vector<mint>;
  if (!mint::can_ntt()) {
    vector<pair<poly, poly>> rationals;
    For(i, len(A)) rationals.emplace_back(poly({wt[i]}), poly({mint(1), -A[i]}));
    return sum_of_rationals(rationals);
  }
  int n = 1;
  while (n < len(A)) n *= 2;
  int k = topbit(n);
  vector<mint> F(n), G(n);
  vector<mint> nxt_F(n), nxt_G(n);
  For(i, len(A)) F[i] = -A[i], G[i] = wt[i];
  int D = 6;

  For(d, k) {
    int b = 1 << d;
    if (d < D) {
      fill(All(nxt_F), mint(0)), fill(All(nxt_G), mint(0));
      for (int L = 0; L < n; L += 2 * b) {
        For(i, b) For(j, b) nxt_F[L + i + j] += F[L + i] * F[L + b + j];
        For(i, b) For(j, b) nxt_G[L + i + j] += F[L + i] * G[L + b + j];
        For(i, b) For(j, b) nxt_G[L + i + j] += F[L + b + i] * G[L + j];
        For(i, b) nxt_F[L + b + i] += F[L + i] + F[L + b + i];
        For(i, b) nxt_G[L + b + i] += G[L + i] + G[L + b + i];
      }
    }
    elif (d == D) {
      for (int L = 0; L < n; L += 2 * b) {
        poly f1 = {F.begin() + L, F.begin() + L + b};
        poly f2 = {F.begin() + L + b, F.begin() + L + 2 * b};
        poly g1 = {G.begin() + L, G.begin() + L + b};
        poly g2 = {G.begin() + L + b, G.begin() + L + 2 * b};
        f1.resize(2 * b), f2.resize(2 * b), g1.resize(2 * b), g2.resize(2 * b);
        ntt(f1, 0), ntt(f2, 0), ntt(g1, 0), ntt(g2, 0);
        For(i, b) f1[i] += 1, f2[i] += 1;
        For(i, b, 2 * b) f1[i] -= 1, f2[i] -= 1;
        For(i, 2 * b) nxt_F[L + i] = f1[i] * f2[i] - 1;
        For(i, 2 * b) nxt_G[L + i] = g1[i] * f2[i] + g2[i] * f1[i];
      }
    }
    else {
      for (int L = 0; L < n; L += 2 * b) {
        poly f1 = {F.begin() + L, F.begin() + L + b};
        poly f2 = {F.begin() + L + b, F.begin() + L + 2 * b};
        poly g1 = {G.begin() + L, G.begin() + L + b};
        poly g2 = {G.begin() + L + b, G.begin() + L + 2 * b};
        ntt_doubling(f1), ntt_doubling(f2), ntt_doubling(g1), ntt_doubling(g2);
        For(i, b) f1[i] += 1, f2[i] += 1;
        For(i, b, 2 * b) f1[i] -= 1, f2[i] -= 1;
        For(i, 2 * b) nxt_F[L + i] = f1[i] * f2[i] - 1;
        For(i, 2 * b) nxt_G[L + i] = g1[i] * f2[i] + g2[i] * f1[i];
      }
    }
    swap(F, nxt_F), swap(G, nxt_G);
  }
  if (k - 1 >= D) ntt(F, 1), ntt(G, 1);
  F.emplace_back(1);
  reverse(All(F)), reverse(All(G));
  F.resize(len(A) + 1);
  G.resize(len(A));
  return {G, F};
}
} // namespace mitsuha
#endif // AJAY_SUM_OF_RATIONALS
