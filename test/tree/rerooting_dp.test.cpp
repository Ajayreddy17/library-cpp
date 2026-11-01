#define PROBLEM "https://judge.yosupo.jp/problem/tree_path_composite_sum"
#include "library/template.hpp"
#include "library/graph/tree_dp/rerooting_dp.hpp"
#include "library/mod/modint.hpp"

using mint = modint998;

void solve() {
  LL(N);
  vector<mint> A(N); read(A);
  Graph<bool, 0> G(N);
  vector<mint> B(N - 1), C(N - 1);
  For(i, N - 1) {
    LL(u, v, b, c);
    G.add(u, v);
    B[i] = b, C[i] = c;
  }
  G.build();
  Tree<decltype(G)> tree(G);

  using Data = pair<mint, mint>;  // cnt, sum
  Data id = {0, 0};
  auto fee = [&](Data x, Data y) -> Data { return {x.first + y.first, x.second + y.second}; };
  auto fev = [&](Data x, int v) -> Data {
    return {x.first + mint(1), x.second + A[v]};
  };
  // e is the directed edge entering v
  auto fve = [&](Data x, int r, int nxt_r) -> Data {
    int eid = tree.get_eid(r, nxt_r);
    x.second = B[eid] * x.second + x.first * C[eid];
    return x;
  };
  Rerooting_DP<decltype(tree), Data> dp(tree, fee, fev, fve, id);

  vector<mint> ANS(N);
  For(v, N) ANS[v] = dp[v].second;
  print(ANS);
}

signed main() {
  solve();
  return 0;
}
