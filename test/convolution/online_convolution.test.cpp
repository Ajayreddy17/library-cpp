#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/online/online_convolution.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void solve() {
  LL(N, M);
  vector<mint> f(N); read(f);
  vector<mint> g(M); read(g);
  f.resize(N + M - 1);
  g.resize(N + M - 1);
  vector<mint> h(N + M - 1);

  Online_Convolution<mint> OC;
  For(i, N + M - 1) { h[i] = OC.query(i, f[i], g[i]); }
  print(h);
}

signed main() {
  solve();

  return 0;
}
