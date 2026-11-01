#ifndef AJAY_COUNT_CYCLE
#define AJAY_COUNT_CYCLE

#include "library/enumerate/bits.hpp"
#include "library/graph/base.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_cycle.hpp

// count cycles per vertex set, N^2 2^N
// cycle length >= 3 (assumes a simple graph)
template <typename T, typename GT>
vector<T> count_cycle(GT& G) {
  const int N = G.N;
  assert(N <= 32 && !GT::is_directed);
  vector<uint32_t> nbd(N);
  For(v, N) for (auto& e : G[v]) nbd[v] |= uint32_t(1) << (e.to);

  vector<T> cyc(1 << N);
  for (int v = 0; v < N; ++v) {
    vector<T> dp(v << v);
    for (int w = 0; w < v; ++w) {
      if (nbd[v] >> w & 1) dp[(v << w) + w] = 1;
    }
    const uint32_t mask = (uint32_t(1) << v) - 1;
    for (uint32_t s = 0; s < (uint32_t(1) << v); ++s) {
      enumerate_all_bit(s, [&](int a) -> void {
        enumerate_all_bit(nbd[a] & mask & (~s), [&](int b) -> void {
          dp[v * (s | 1 << b) + b] += dp[v * s + a];
        });
        if (popcount(s) >= 2 && nbd[a] >> v & 1) cyc[s | 1 << v] += dp[v * s + a];
      });
    }
  }
  for (auto& x : cyc) x /= T(2);
  return cyc;
}
} // namespace mitsuha
#endif // AJAY_COUNT_CYCLE
