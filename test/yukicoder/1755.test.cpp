#define PROBLEM "https://yukicoder.me/problems/no/1755"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/seq/find_linear_rec.hpp"
#include "library/poly/fps_inv.hpp"

using mint = modint998;

void solve() {
  mint K = 26;

  auto f = [&](int N) -> mint {
    mint ANS = 0;
    For(a, N + 1) {
      long long n = N - 2 * a;
      if (n <= 1) break;
      mint x = K.pow(cld<long long>(n - 1, 2)) * (K - 1);
      mint y = (n % 2 == 0 ? K * (K - mint(1)) : 0);
      x = x + x - y;
      x *= K.pow(a);
      ANS += x;
    }
    return ANS;
  };

  long long LIM = 20;
  vector<mint> A(LIM);
  For(n, LIM) A[n] = f(n);
  auto G = find_linear_rec<mint>(A);
  auto F = convolution(A, G);
  F.resize(6);

  LIM = 1000000;
  G.resize(LIM + 1);
  G = fps_inv(G);
  F = convolution(F, G);

  LL(T);
  For(T) {
    LL(N);
    print(F[N]);
  }
}

signed main() {
  solve();
  return 0;
}
