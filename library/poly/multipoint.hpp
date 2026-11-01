#ifndef AJAY_MULTIPOINT
#define AJAY_MULTIPOINT

#include "library/poly/middle_product.hpp"
#include "library/mod/all_inverse.hpp"
#include "library/poly/fps_div.hpp"
#include "library/poly/ntt_doubling.hpp"
#include "library/poly/transposed_ntt.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/poly/multipoint.hpp


template <typename mint>
struct SubproductTree {
  int m;
  int sz;
  vector<vector<mint>> T;
  SubproductTree(const vector<mint>& x) {
    m = len(x);
    sz = 1;
    while (sz < m) sz *= 2;
    T.resize(2 * sz);
    For(i, sz) T[sz + i] = {1, (i < m ? -x[i] : 0)};
    Frr(i, 1, sz) T[i] = convolution(T[2 * i], T[2 * i + 1]);
  }

  vector<mint> evaluation(vector<mint> f) {
    int n = len(f);
    if (n == 0) return vector<mint>(m, mint(0));
    f.resize(2 * n - 1);
    vector<vector<mint>> g(2 * sz);
    g[1] = T[1];
    g[1].resize(n);
    g[1] = fps_inv(g[1]);
    g[1] = middle_product(f, g[1]);
    g[1].resize(sz);

    For(i, 1, sz) {
      g[2 * i] = middle_product(g[i], T[2 * i + 1]);
      g[2 * i + 1] = middle_product(g[i], T[2 * i]);
    }
    vector<mint> vals(m);
    For(i, m) vals[i] = g[sz + i][0];
    return vals;
  }

  vector<mint> interpolation(const vector<mint>& y) {
    assert(len(y) == m);
    vector<mint> a(m);
    For(i, m) a[i] = T[1][m - i - 1] * (i + 1);

    a = evaluation(a);
    for (auto& v : a) assert(v != mint(0));
    vector<vector<mint>> t(2 * sz);
    For(i, sz) t[sz + i] = {(i < m ? y[i] / a[i] : 0)};
    Frr(i, 1, sz) {
      t[i] = convolution(t[2 * i], T[2 * i + 1]);
      auto tt = convolution(t[2 * i + 1], T[2 * i]);
      For(k, len(t[i])) t[i][k] += tt[k];
    }
    t[1].resize(m);
    reverse(t[1].begin(), t[1].end());
    return t[1];
  }
};

template <typename mint>
vector<mint> multipoint_evaluation_ntt(vector<mint> f, const vector<mint>& point) {
  if (f.empty()) return vector<mint>(len(point), 0);
  using poly = vector<mint>;
  int n = 1, k = 0;
  while (n < len(point)) n *= 2, ++k;
  vector<vector<mint>> F(k + 1, vector<mint>(2 * n));
  For(i, len(point)) F[0][2 * i] = -point[i];

  For(d, k) {
    int b = 1 << d;
    for (int L = 0; L < 2 * n; L += 4 * b) {
      poly f1 = {F[d].begin() + L, F[d].begin() + L + b};
      poly f2 = {F[d].begin() + L + 2 * b, F[d].begin() + L + 3 * b};
      ntt_doubling(f1), ntt_doubling(f2);
      For(i, b) f1[i] += 1, f2[i] += 1;
      For(i, b, 2 * b) f1[i] -= 1, f2[i] -= 1;
      copy(f1.begin(), f1.end(), F[d].begin() + L);
      copy(f2.begin(), f2.end(), F[d].begin() + L + 2 * b);
      For(i, 2 * b) { F[d + 1][L + i] = f1[i] * f2[i] - 1; }
    }
  }
  vector<mint> P = {F[k].begin(), F[k].begin() + n};
  ntt(P, 1), P.emplace_back(1), reverse(P.begin(), P.end()), P.resize(len(f)), P = fps_inv<mint>(P);
  f.resize(n + len(P) - 1), f = middle_product<mint>(f, P), reverse(f.begin(), f.end());
  transposed_ntt(f, 1);
  vector<mint>& G = f;
  Frr(d, k) {
    vector<mint> nxt_G(n);
    int b = 1 << d;
    for (int L = 0; L < n; L += 2 * b) {
      vector<mint> g1(2 * b), g2(2 * b);
      For(i, 2 * b) { g1[i] = G[L + i] * F[d][2 * L + 2 * b + i]; }
      For(i, 2 * b) { g2[i] = G[L + i] * F[d][2 * L + i]; }
      ntt_doubling<mint, true>(g1), ntt_doubling<mint, true>(g2);
      For(i, b) { nxt_G[L + i] = g1[i], nxt_G[L + b + i] = g2[i]; }
    }
    swap(G, nxt_G);
  }
  G.resize(len(point));
  return G;
}

template <typename mint>
vector<mint> multipoint_eval(const vector<mint>& f, const vector<mint>& x) {
  if (x.empty()) return {};
  if (mint::can_ntt()) return multipoint_evaluation_ntt(f, x);
  SubproductTree<mint> F(x);
  return F.evaluation(f);
}

template <typename mint>
vector<mint> multipoint_interpolate(const vector<mint>& x, const vector<mint>& y) {
  assert(len(x) == len(y));
  if (x.empty()) return {};
  SubproductTree<mint> F(x);
  return F.interpolation(y);
}

// calculate f(ar^k) for 0 <= k < m
template <typename mint>
vector<mint> multipoint_eval_on_geom_seq(vector<mint> f, mint a, mint r, int m) {
  assert(m >= 0);
  const int n = len(f);
  if (m == 0) return {};
  if (n == 0) return vector<mint>(m, 0);

  auto eval = [&](mint x) -> mint {
    mint fx = 0;
    mint pow = 1;
    For(i, n) fx += f[i] * pow, pow *= x;
    return fx;
  };

  if (r == mint(0)) {
    vector<mint> res(m);
    For(i, 1, m) res[i] = f[0];
    res[0] = eval(a);
    return res;
  }
  if (n < 60 || m < 60) {
    vector<mint> res(m);
    For(i, m) res[i] = eval(a), a *= r;
    return res;
  }
  assert(r != mint(0));
  // reduce to a == 1
  mint pow_a = 1;
  For(i, n) f[i] *= pow_a, pow_a *= a;

  auto calc = [&](mint r, int m) -> vector<mint> {
    // compute r^{t_i}
    vector<mint> res(m);
    mint pow = 1;
    res[0] = 1;
    For(i, m - 1) {
      res[i + 1] = res[i] * pow;
      pow *= r;
    }
    return res;
  };

  vector<mint> A = calc(r, n + m - 1), B = calc(r.inverse(), max(n, m));
  For(i, n) f[i] *= B[i];
  f = middle_product(A, f);
  For(i, m) f[i] *= B[i];
  return f;
}

// Y[i] = f(ar^i)
template <typename mint>
vector<mint> multipoint_interpolate_on_geom_seq(vector<mint> Y, mint a, mint r) {
  const int n = len(Y);
  if (n == 0) return {};
  if (n == 1) return {Y[0]};
  assert(r != mint(0));
  assert(a != mint(0));
  mint ir = r.inverse();

  vector<mint> POW(n + n - 1), tPOW(n + n - 1);
  POW[0] = tPOW[0] = mint(1);
  For(i, n + n - 2) POW[i + 1] = POW[i] * r, tPOW[i + 1] = tPOW[i] * POW[i];

  vector<mint> iPOW(n + n - 1), itPOW(n + n - 1);
  iPOW[0] = itPOW[0] = mint(1);
  For(i, n) iPOW[i + 1] = iPOW[i] * ir, itPOW[i + 1] = itPOW[i] * iPOW[i];

  // prod_[1,i] 1-r^k
  vector<mint> S(n);
  S[0] = mint(1);
  For(i, 1, n) S[i] = S[i - 1] * (mint(1) - POW[i]);
  assert(S[n - 1] != mint(0));
  vector<mint> iS = all_inverse<mint>(S);
  mint sn = S[n - 1] * (mint(1) - POW[n]);

  For(i, n) {
    Y[i] = Y[i] * tPOW[n - 1 - i] * itPOW[n - 1] * iS[i] * iS[n - 1 - i];
    if (i % 2 == 1) Y[i] = -Y[i];
  }

  // sum_i Y[i] / 1-r^ix
  For(i, n) Y[i] *= itPOW[i];
  vector<mint> f = middle_product(tPOW, Y);
  For(i, n) f[i] *= itPOW[i];

  // prod 1-r^ix
  vector<mint> g(n);
  g[0] = mint(1);
  For(i, 1, n) {
    g[i] = tPOW[i] * sn * iS[i] * iS[n - i];
    if (i % 2 == 1) g[i] = -g[i];
  }
  f = convolution<mint>(f, g);
  f.resize(n);

  reverse(f.begin(), f.end());
  mint ia = a.inverse();
  mint pow = 1;
  For(i, n) f[i] *= pow, pow *= ia;
  return f;
}
} // namespace mitsuha
#endif // AJAY_MULTIPOINT
