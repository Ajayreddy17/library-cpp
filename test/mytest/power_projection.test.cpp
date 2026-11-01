#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/random/base.hpp"
#include "library/poly/power_projection.hpp"
#include "library/mod/modint.hpp"
#include "library/poly/fps_pow.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  For(100) {
    For(N, 10) For(K, 10) {
      vector<mint> f(K + 1);
      vector<mint> wt(K + 1);
      For(k, K + 1) f[k] = RNG(0, 5), wt[k] = RNG(0, 5);
      vector<mint> A(N + 1);
      For(n, N + 1) {
        vector<mint> h = fps_pow<mint>(f, n);
        For(i, K + 1) A[n] += wt[i] * h[i];
      }
      vector<mint> B = power_projection<mint>(wt, f, N);
      assert(A == B);
    }
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  test();
  solve();
}
