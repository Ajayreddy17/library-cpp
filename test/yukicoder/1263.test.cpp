#define PROBLEM "https://yukicoder.me/problems/no/1263"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/enumerate/partition.hpp"

using mint = modint107;

void solve() {
  mint ANS = 1;
  LL(N);
  auto f = [&](vector<int> P) -> void {
    mint v = fact<mint>(N);
    for (auto&& x: P) v *= inv<mint>(x) * (x % 2 == 0 ? mint(-1) : mint(1));
    vector<int> CNT(N + 1);
    for (auto&& x: P) ++CNT[x];
    for (auto&& x: CNT) v *= fact_inv<mint>(x);
    ANS *= v;
  };
  enumerate_partition(N, f);
  print(ANS);
}

signed main() {
  cout << fixed << setprecision(15);

  long long T = 1;
  // LL(T);
  For(T) solve();

  return 0;
}
