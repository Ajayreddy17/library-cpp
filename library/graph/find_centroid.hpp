#ifndef AJAY_FIND_CENTROID
#define AJAY_FIND_CENTROID

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/find_centroid.hpp

// (v,w) or (v,-1)
template <typename GT>
pair<int, int> find_centroids(GT& G, vector<long long> weight = {}) {
  int N = G.N;
  if (weight.empty()) weight.assign(N, 1);
  vector<int> par(N, -1);
  vector<int> V(N);
  vector<long long> sz(N);
  int l = 0, r = 0;
  V[r++] = 0;
  while (l < r) {
    int v = V[l++];
    for (auto&& e : G[v])
      if (e.to != par[v]) {
        par[e.to] = v;
        V[r++] = e.to;
      }
  }
  Frr(i, N) {
    int v = V[i];
    sz[v] += weight[v];
    int p = par[v];
    if (p != -1) sz[p] += sz[v];
  }

  long long FULL = SUM<long long>(weight);
  long long M = FULL / 2;
  auto check = [&](int v) -> bool {
    if (FULL - sz[v] > M) return false;
    for (auto&& e : G[v]) {
      if (e.to != par[v] && sz[e.to] > M) return false;
    }
    return true;
  };
  pair<int, int> ANS = {-1, -1};
  For(v, N) if (check(v)) {
    if (ANS.first != -1) {
      ANS.second = v;
    } else {
      ANS.first = v;
    }
  }
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_FIND_CENTROID
