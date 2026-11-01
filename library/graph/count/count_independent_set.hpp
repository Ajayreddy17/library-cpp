#ifndef AJAY_COUNT_INDEPENDENT_SET
#define AJAY_COUNT_INDEPENDENT_SET

#include "library/graph/path_cycle_decomposition.hpp"
#include "library/poly/convolution.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_independent_set.hpp

// count independent sets, including the empty set; about N 1.381^N
template <typename GT>
uint64_t count_independent_set(GT& G) {
  using U = uint64_t;
  const int N = G.N;
  assert(N < 64);
  if (N == 0) return 1;
  vector<U> nbd(N);
  For(v, N) for (auto&& e : G[v]) nbd[v] |= U(1) << e.to;

  vector<U> dp_path(N + 1), dp_cyc(N + 1);
  dp_path[0] = 1, dp_path[1] = 2;
  For(i, 2, N + 1) dp_path[i] = dp_path[i - 1] + dp_path[i - 2];
  For(i, 3, N + 1) dp_cyc[i] = dp_path[i - 1] + dp_path[i - 3];

  auto dfs = [&](auto& dfs, U s) -> U {
    int deg0 = 0;
    pair<int, int> p = {-1, -1};  // (v, d)
    For(v, N) if (s >> v & 1) {
      int d = popcnt(nbd[v] & s);
      if (chmax(p.second, d)) p.first = v;
      if (d == 0) {
        ++deg0;
        s &= ~(U(1) << v);
      }
    }
    if (s == 0) return U(1) << deg0;
    int v = p.first;
    if (p.second >= 3) {
      s &= ~(U(1) << v);
      return (dfs(dfs, s) + dfs(dfs, s & ~nbd[v])) << deg0;
    }
    // d <= 2: only paths and cycles
    vector<int> V;
    For(v, N) if (s >> v & 1) V.emplace_back(v);
    int n = len(V);
    Graph<bool, 0> G(n);
    For(i, n) {
      U x = nbd[V[i]] & s;
      while (x) {
        int v = topbit(x);
        x ^= U(1) << v;
        int j = LB(V, v);
        if (i < j) G.add(i, j);
      }
    }
    G.build();
    auto [paths, cycs] = path_cycle_decomposition(G);
    U res = 1;
    for (auto&& P : paths) res *= dp_path[len(P)];
    for (auto&& C : cycs) res *= dp_cyc[len(C)];
    return res << deg0;
  };
  return dfs(dfs, (U(1) << N) - 1);
}

// count independent sets, including the empty set; about N 1.381^N
template <typename GT>
vector<uint64_t> count_independent_set_by_size(GT& G) {
  using U = uint64_t;
  const int N = G.N;
  assert(N < 64);
  if (N == 0) return {1};
  vector<U> nbd(N);
  For(v, N) for (auto&& e : G[v]) nbd[v] |= U(1) << e.to;

  vector<vector<U>> dp_path(N + 1), dp_cyc(N + 1);
  dp_path[0] = {1}, dp_path[1] = {1, 1};
  For(i, 2, N + 1) {
    dp_path[i] = dp_path[i - 1];
    dp_path[i].resize(cld<int>(i, 2) + 1);
    For(k, len(dp_path[i - 2])) { dp_path[i][k + 1] += dp_path[i - 2][k]; }
  }
  For(i, 3, N + 1) {
    dp_cyc[i] = dp_path[i - 1];
    For(k, len(dp_path[i - 3])) dp_cyc[i][k + 1] += dp_path[i - 3][k];
  }

  auto dfs = [&](auto& dfs, U s) -> vector<U> {
    vector<U> res = {1};
    pair<int, int> p = {-1, -1};  // (v, d)
    For(v, N) if (s >> v & 1) {
      int d = popcnt(nbd[v] & s);
      if (chmax(p.second, d)) p.first = v;
      if (d == 0) {
        res.emplace_back(0);
        Frr(i, len(res) - 1) res[i + 1] += res[i];
        s &= ~(U(1) << v);
      }
    }
    if (s == 0) return res;
    int v = p.first;
    if (p.second >= 3) {
      s &= ~(U(1) << v);
      auto f = dfs(dfs, s), g = dfs(dfs, s & ~nbd[v]);
      if (len(f) < len(g) + 1) f.resize(len(g) + 1);
      For(i, len(g)) f[i + 1] += g[i];
      return convolution_naive(f, res);
    }
    // d <= 2: only paths and cycles
    vector<int> V;
    For(v, N) if (s >> v & 1) V.emplace_back(v);
    int n = len(V);
    Graph<bool, 0> G(n);
    For(i, n) {
      U x = nbd[V[i]] & s;
      while (x) {
        int v = topbit(x);
        x ^= U(1) << v;
        int j = LB(V, v);
        if (i < j) G.add(i, j);
      }
    }
    G.build();
    auto [paths, cycs] = path_cycle_decomposition(G);
    for (auto&& P : paths) res = convolution_naive(res, dp_path[len(P)]);
    for (auto&& C : cycs) res = convolution_naive(res, dp_cyc[len(C)]);
    return res;
  };
  auto res = dfs(dfs, (U(1) << N) - 1);
  res.resize(N + 1);
  return res;
}

// weight is the product of vertex weights
// https://codeforces.com/contest/468/problem/E
template <typename T, typename GT>
vector<T> count_independent_set_by_size_weighted(GT& G, vector<T> wt) {
  using U = uint64_t;
  const int N = G.N;
  assert(N < 64);
  if (N == 0) return {1};
  vector<U> nbd(N);
  For(v, N) for (auto&& e : G[v]) nbd[v] |= U(1) << e.to;

  auto solve_path = [&](const vector<T>& A) -> vector<T> {
    int N = len(A);
    vector<vector<T>> dp(2, vector<T>(cld<int>(N, 2) + 2));
    dp[0][0] = 1;
    For(i, N) {
      Frr(j, cld<int>(i, 2) + 1) {
        T a = dp[0][j];
        T b = dp[1][j];
        dp[0][j] = a + b, dp[1][j] = 0;
        dp[1][j + 1] += a * A[i];
      }
    }
    vector<T> f(cld<int>(N, 2) + 1);
    For(j, len(f)) f[j] = dp[0][j] + dp[1][j];
    return f;
  };
  auto solve_cycle = [&](const vector<T>& A) -> vector<T> {
    int N = len(A);
    vector<vector<vector<T>>> dp(2, vector<vector<T>>(2, vector<T>(cld<int>(N, 2) + 2)));
    dp[0][0][0] = 1;
    dp[1][1][1] = A[0];
    For(i, 1, N) {
      For(k, 2) {
        Frr(j, cld<int>(i, 2) + 1) {
          T a = dp[k][0][j];
          T b = dp[k][1][j];
          dp[k][0][j] = a + b, dp[k][1][j] = 0;
          dp[k][1][j + 1] += a * A[i];
        }
      }
    }
    vector<T> f(N / 2 + 1);
    For(k, N / 2 + 1) { f[k] = dp[0][0][k] + dp[0][1][k] + dp[1][0][k]; }
    return f;
  };

  auto dfs = [&](auto& dfs, U s) -> vector<T> {
    vector<T> res = {1};
    pair<int, int> p = {-1, -1};  // (v, d)
    For(v, N) if (s >> v & 1) {
      int d = popcnt(nbd[v] & s);
      if (chmax(p.second, d)) p.first = v;
      if (d == 0) {
        res.emplace_back(0);
        Frr(i, len(res) - 1) res[i + 1] += res[i] * wt[v];
        s &= ~(U(1) << v);
      }
    }
    if (s == 0) return res;
    int v = p.first;
    if (p.second >= 3) {
      s &= ~(U(1) << v);
      auto f = dfs(dfs, s), g = dfs(dfs, s & ~nbd[v]);
      if (len(f) < len(g) + 1) f.resize(len(g) + 1);
      For(i, len(g)) f[i + 1] += g[i] * wt[v];
      return convolution<T>(f, res);
    }
    // d <= 2: only paths and cycles
    vector<int> V;
    For(v, N) if (s >> v & 1) V.emplace_back(v);
    int n = len(V);
    Graph<bool, 0> G(n);
    For(i, n) {
      U x = nbd[V[i]] & s;
      while (x) {
        int v = topbit(x);
        x ^= U(1) << v;
        int j = LB(V, v);
        if (i < j) G.add(i, j);
      }
    }
    G.build();
    auto [paths, cycs] = path_cycle_decomposition(G);
    for (auto&& P : paths) {
      vector<T> A;
      for (auto& i : P) A.emplace_back(wt[V[i]]);
      res = convolution<T>(res, solve_path(A));
    }
    for (auto&& P : cycs) {
      vector<T> A;
      for (auto& i : P) A.emplace_back(wt[V[i]]);
      res = convolution(res, solve_cycle(A));
    }
    return res;
  };
  auto res = dfs(dfs, (U(1) << N) - 1);
  res.resize(N + 1);
  return res;
}
} // namespace mitsuha
#endif // AJAY_COUNT_INDEPENDENT_SET
