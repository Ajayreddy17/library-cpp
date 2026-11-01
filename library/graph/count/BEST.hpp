#ifndef AJAY_BEST
#define AJAY_BEST

#include "library/graph/base.hpp"
#include "library/linear_algebra/blackbox/det.hpp"
#include "library/linear_algebra/det.hpp"
#include "library/mod/modint_common.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/BEST.hpp

/*
Count closed walks through every edge starting from one chosen edge.
Multi-edges are passed as vector<int>(eid); all 1 if absent. e.cost is ignored.
Edges are labelled; to identify parallel edges, divide by factorials afterwards.
O(N^2+NM) (+ multiply by factorials of multiplicities at the end).
*/
template <typename mint, bool sparse, typename GT>
mint BEST_theorem(GT G, vector<int> edge_multiplicity = {}) {
  static_assert(GT::is_directed);
  int N = G.N, M = G.M;
  if (M == 0) return 0;
  if (edge_multiplicity.empty()) edge_multiplicity.assign(M, 1);
  vector<int> vs;
  for (auto& e: G.edges) {
    if (edge_multiplicity[e.id] == 0) continue;
    vs.emplace_back(e.frm), vs.emplace_back(e.to);
  }

  settify(vs);
  G = G.rearrange(vs, true);
  N = G.N;

  vector<int> indeg(N), outdeg(N);
  if constexpr (sparse) {
    vector<tuple<int, int, mint>> mat;
    for (auto& e: G.edges) {
      int a = e.frm, b = e.to, x = edge_multiplicity[e.id];
      outdeg[a] += x, indeg[b] += x;
      if (a < N - 1 && b < N - 1) mat.emplace_back(a, b, -x);
      if (a < N - 1) mat.emplace_back(a, a, x);
    }
    For(v, N) if (indeg[v] != outdeg[v]) return 0;

    auto apply = [&](vector<mint> A) -> vector<mint> {
      vector<mint> B(N - 1);
      for (auto& [a, b, c]: mat) B[b] += A[a] * c;
      return B;
    };
    mint d = blackbox_det<mint>(N - 1, apply);
    for (auto& x: outdeg) { d *= fact<mint>(x - 1); }
    return d;
  } else {
    // dense
    vector<vector<mint>> mat(N - 1, vector<mint>(N - 1));
    for (auto& e: G.edges) {
      int a = e.frm, b = e.to, x = edge_multiplicity[e.id];
      outdeg[a] += x, indeg[b] += x;
      if (a < N - 1 && b < N - 1) mat[a][b] -= x;
      if (a < N - 1) mat[a][a] += x;
    }
    For(v, N) if (indeg[v] != outdeg[v]) return 0;
    mint d = det(mat);
    for (auto& x: outdeg) { d *= fact<mint>(x - 1); }
    return d;
  }
}
} // namespace mitsuha
#endif // AJAY_BEST
