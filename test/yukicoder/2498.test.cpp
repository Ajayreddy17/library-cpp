#define PROBLEM "https://yukicoder.me/problems/no/2498"
#include "library/template.hpp"
#include "library/datastructure/segment_tree/dual_segtree.hpp"
#include "library/mod/modint.hpp"
#include "library/monoid/bitwise_function.hpp"

using mint = modint998;

void solve() {
  LL(N, Q);
  vector<uint32_t> A(N); read(A);
  using Mono = Bitwise_Function<uint32_t>;
  Dual_SegTree<Mono> seg(N);
  For(Q) {
    STR(S);
    INT(L, R, x);
    --L;
    if (S == "o") { seg.apply(L, R, Mono::func_or(x)); }
    if (S == "x") { seg.apply(L, R, Mono::func_xor(x)); }
  }

  auto F = seg.get_all();

  // 各項の popcnt ごとの数え
  const int K = 30;
  array<mint, K + 1> DP{};
  DP[0] = 1;

  For(i, N) {
    auto [F0, F1] = F[i];
    // [0, A[i]]
    struct Data {
      uint64_t lo = 0, eq = 0;
    };
    vector<Data> dp(K + 1);
    dp[0].eq = 1;
    Frr(k, K) {
      int x0 = F0 >> k & 1;
      int x1 = F1 >> k & 1;
      vector<Data> newdp(K + 1);
      For(n, K) {
        if (A[i] >> k & 1) {
          newdp[n + x0].lo += dp[n].lo + dp[n].eq;
          newdp[n + x1].lo += dp[n].lo;
          newdp[n + x1].eq += dp[n].eq;
        } else {
          newdp[n + x0].lo += dp[n].lo;
          newdp[n + x0].eq += dp[n].eq;
          newdp[n + x1].lo += dp[n].lo;
        }
      }
      swap(dp, newdp);
    }
    array<mint, K + 1> newDP{};
    For(a, K + 1) For(b, K + 1) {
      newDP[max(a, b)] += DP[a] * mint(dp[b].lo + dp[b].eq);
    }
    swap(DP, newDP);
  }

  mint ANS = 0;
  For(k, K + 1) ANS += mint(k) * DP[k];
  print(ANS);
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
