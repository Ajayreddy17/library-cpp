#ifndef AJAY_O_NK_REROOTING_TREE_DP
#define AJAY_O_NK_REROOTING_TREE_DP

#include "library/graph/base.hpp"
#include "library/datastructure/offset_vector.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tree_dp/O_NK_rerooting_tree_dp.hpp

// O(NK) rerooting for square-type tree DPs
// when only the size-K DP value rooted at each vertex v is needed
// https://codeforces.com/contest/2229/problem/I
//
// f_ee(A, B, L, R)
// f_ev(A, v, L, R)
// f_ve(A, v, p, L, R)
// get_ANS(A, v)
// A, B and the result are Offset_Vector<T>; the result covers [L,R)
//
// f_ee: merge virtual trees
// f_ev: add a root to a virtual tree to get a real tree
// f_ve: add the parent edge to a real tree to get a virtual tree
// get_ANS: answer from a virtual tree and v. [K-1,K]
template <typename GT, typename T, typename F1, typename F2, typename F3,
          typename F4>
vector<T> O_NK_rerooting_tree_dp(GT& G, int K, F1 f_ee, F2 f_ev, F3 f_ve,
                             F4 get_ANS, const T id) {
  int N = G.N;
  vector<int> V(N), par(N, -1), sz(N, 1);
  vector<vector<int>> ch(N);
  {
    int t = 0;
    V[t++] = 0;
    For(i, N) {
      int v = V[i];
      for (auto& e : G[v]) {
        if (e.to == par[v]) continue;
        V[t++] = e.to;
        par[e.to] = v;
        ch[v].emplace_back(e.to);
      }
    }
    Frr(i, 1, N) {
      int v = V[i];
      sz[par[v]] += sz[v];
    }
  }

  /*
  dp_down[v]: virtual tree at v.
  dp_prefix[c]: prefix virtual tree at v, before c. (c: not included)
  dp_up[v]: virtual tree at v, upward.
  */

  using D = OffsetVector<T>;
  vector<D> dp_down(N), dp_prefix(N), dp_up(N);
  vector<T> ANS(N);

  // calc dp_down, dp_prefix
  Frr(i, N) {
    int v = V[i];
    D X(0, 1, id);
    int n = 0;
    for (int c : ch[v]) {
      dp_prefix[c] = X;
      D Y = f_ev(dp_down[c], c, 0, min<int>(K, sz[c]) + 1);
      Y = f_ve(Y, c, v, 0, min<int>(K, sz[c]) + 1);
      n += sz[c];
      X = f_ee(X, Y, 0, min<int>(n, K) + 1);
    }
    dp_down[v] = X;
  }

  dp_up[0] = D(0, 1, id);
  For(i, N) {
    int v = V[i];
    int nc = len(ch[v]);
    D X = dp_up[v];
    int s = sz[v];
    Frr(k, nc) {
      int c = ch[v][k];
      D Y = f_ee(X, dp_prefix[c], max(K - sz[c] - 1, 0), K + 1);
      Y = f_ev(Y, v, max(K - sz[c], 0), K + 1);
      dp_up[c] = f_ve(Y, v, c, max(K - sz[c], 0), K + 1);
      s -= sz[c];
      Y = f_ev(dp_down[c], c, 0, min(K, sz[c]) + 1);
      Y = f_ve(Y, c, v, 0, min(K, sz[c]) + 1);
      X = f_ee(X, Y, max(K - s, 0), K + 1);
    }
    ANS[v] = get_ANS(X, v);
  }
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_O_NK_REROOTING_TREE_DP
