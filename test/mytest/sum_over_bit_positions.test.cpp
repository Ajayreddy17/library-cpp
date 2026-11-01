#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/datastructure/bit_array.hpp"
#include "library/random/base.hpp"
#include "library/datastructure/sum_over_bit_positions.hpp"
#include "library/util/maspy_compat.hpp"

void test(int N, int Q, bool check = true) {
  vector<int> A(N);
  Bit_Array bs(N);
  For(i, N) A[i] = RNG(0, N), bs[i] = RNG(0, 2);
  Sum_Over_Bit_Positions<int, 100'000> SB(N, [&](int i) -> int { return A[i]; });

  For(Q) {
    For(100) {
      int i = RNG(0, N);
      bs[i].flip();
    }
    long long ans = SB.query<long long>(bs, 0, N);
    if (check) {
      long long god = 0;
      For(i, N) if (bs[i]) god += A[i];
      assert(ans == god);
    }
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  // test(100'000, 10'000, false);
  For(N, 1, 150) For(Q, 1, 150) test(N, Q, true);
  solve();
  return 0;
}
