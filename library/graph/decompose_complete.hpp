#ifndef AJAY_DECOMPOSE_COMPLETE
#define AJAY_DECOMPOSE_COMPLETE

#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/decompose_complete.hpp
// decompose the complete graph on an even number of vertices into Hamiltonian paths
vector<vector<int>> to_paths(long long N, long long K) {
  assert(N >= 0 && N % 2 == 0 && 0 <= K && K <= N / 2);
  vector<vector<int>> paths(K, vector<int>(N));
  For(k, K) For(n, N) {
    auto &x = paths[k][n];
    x = k + (n & 1 ? (n + 1) / 2 : -(n / 2));
    if (x < 0) x += N;
    if (x >= N) x -= N;
  }
  return paths;
}

// decompose the complete graph on an odd number of vertices into Hamiltonian cycles
vector<vector<int>> to_cycles(long long N, long long K) {
  assert(N >= 0 && N % 2 == 1 && 0 <= K && K <= (N - 1) / 2);
  auto paths = to_paths(N - 1, K);
  for (auto &&P: paths) P.emplace_back(N - 1);
  return paths;
}

// https://codeforces.com/contest/12/problem/E
// decompose the complete graph on an even number of vertices into perfect matchings
// i.e. an edge colouring
// any 2 of them together form a Hamiltonian cycle
vector<vector<pair<int, int>>> to_matchings(long long N) {
  assert(N > 0 && N % 2 == 0);
  vector<vector<pair<int, int>>> res(N - 1);
  const int mod = N - 1;
  For(a, mod) {
    res[a].reserve(N / 2);
    res[a].emplace_back(N - 1, a);
    int x = a, y = a;
    For(N / 2 - 1) {
      --x, ++y;
      if (x < 0) x += N - 1;
      if (y >= N - 1) y -= N - 1;
      res[a].emplace_back(x, y);
    }
  }
  return res;
}

// decompose the complete graph on an even number of vertices into Hamiltonian cycles and one matching
pair<vector<vector<long long>>, vector<long long>> to_cycles_and_matching(long long N) {
  assert(N > 0 && N % 2 == 0);
  vector<vector<long long>> cycles(N / 2 - 1, vector<long long>(N));
  long long mod = N - 1;
  For(a, mod - 1) if (a % 2 == 0) {
    auto &C = cycles[a / 2];
    // connect when the sum is 2a or 2a+2
    C[0] = a;
    For(i, N - 2) {
      long long nxt = (i % 2 == 0 ? 2 * a + 2 - C[i] : 2 * a - C[i]);
      if (nxt < 0) nxt += mod;
      if (nxt >= mod) nxt -= mod;
      C[i + 1] = nxt;
    }
    C[N - 1] = mod;
  }
  vector<long long> match(N);
  For(a, mod / 2) {
    long long b = mod - 2 - a;
    match[2 * a] = a, match[2 * a + 1] = b;
  }
  match[N - 2] = N - 2, match[N - 1] = N - 1;
  return {cycles, match};
}
} // namespace mitsuha
#endif // AJAY_DECOMPOSE_COMPLETE
