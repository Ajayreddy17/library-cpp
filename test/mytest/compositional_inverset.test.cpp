#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/poly/compositional_inverse.hpp"
#include "library/poly/compositional_inverse_old.hpp"
#include "library/poly/composition.hpp"
#include "library/random/base.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

void test() {
  For(N, 100) {
    vector<mint> f(N);
    For(i, 1, N) f[i] = RNG(1, 10);
    vector<mint> A = compositional_inverse<mint>(f);
    vector<mint> B = compositional_inverse_old<mint>(f);
    assert(A == B);
    vector<mint> F = composition<mint>(f, A);
    vector<mint> G = composition<mint>(A, f);
    For(i, N) {
      if (i == 1) {
        assert(F[i] == mint(1));
        assert(G[i] == mint(1));
      } else {
        assert(F[i] == mint(0));
        assert(G[i] == mint(0));
      }
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
