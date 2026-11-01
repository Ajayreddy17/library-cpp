#define PROBLEM "https://yukicoder.me/problems/no/1262"
#include "library/template.hpp"
#include "library/number/euler_phi.hpp"

void solve() {
  LL(N, M);
  vector<long long> A(M); read(A);
  auto phi = euler_phi_table<int>(N);
  vector<long long> F(N + 1);
  For(d, 1, N + 1) {
    long long x = d;
    while (x <= N) {
      F[x] += phi[x / d] * d;
      x += d;
    }
  }
  long long ANS = 0;
  for (auto&& a: A) ANS += F[a] - a;
  print(ANS);
}

signed main() {
  solve();
  return 0;
}
