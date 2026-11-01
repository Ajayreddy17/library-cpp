#ifndef AJAY_OUTERPLANAR
#define AJAY_OUTERPLANAR

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/outerplanar.hpp

// assumes N>=3 and simple. return: biconnected planar?
bool check_outerplanar(vector<int> cycle, Graph<int, 0> G) {
  int N = G.N;
  if (N != len(cycle)) return {};
  G = G.rearrange(cycle);
  vector<pair<int, int>> LR;
  vector<bool> exist(N);
  for (auto& e : G.edges) {
    int a = e.frm, b = e.to;
    if (a > b) swap(a, b);
    if (b == (a + 1) % N) {
      exist[a] = 1;
    } else {
      LR.emplace_back(a, b);
    }
  }
  sort(All(LR), [&](auto& a, auto& b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second > b.second;
  });

  vector<int> st;
  for (auto [l, r] : LR) {
    while (!st.empty() && st.back() < l) st.pop_back();
    if (!st.empty()) {
      if (l < st.back() && st.back() < r) return false;
    }
    st.emplace_back(r);
  }
  return true;
}

// N>=3. simple biconnected.
// https://codeforces.com/contest/1656/problem/I
vector<int> hamiltonian_cycle_outerplanar(Graph<int, 0>& G) {
  int N = G.N;
  assert(N >= 3);
  vector<set<int>> adj(N);
  for (auto& e : G.edges) {
    adj[e.frm].emplace(e.to), adj[e.to].emplace(e.frm);
  }
  vector<bool> exist(N, 1);
  vector<int> que;
  For(v, N) if (len(adj[v]) == 2) que.emplace_back(v);

  vector<tuple<int, int, int>> history;
  int n = N;
  while (n > 2) {
    if (que.empty()) return {};
    int x = POP(que);
    if (!exist[x] || len(adj[x]) != 2) continue;
    int a = *(adj[x].begin());
    int b = *(next(adj[x].begin()));
    if (a == x || b == x || a == b) return {};
    history.emplace_back(x, a, b);
    adj[x].clear(), exist[x] = 0, --n;
    adj[a].erase(x), adj[b].erase(x);
    adj[a].emplace(b), adj[b].emplace(a);
    que.emplace_back(a), que.emplace_back(b);
  }
  int u = -1, v = -1;
  For(i, N) {
    if (exist[i]) (u == -1 ? u : v) = i;
  }

  vector<int> nxt(N, -1), pre(N, -1);
  nxt[u] = v, pre[u] = v;
  nxt[v] = u, pre[v] = u;

  while (len(history)) {
    auto [x, a, b] = POP(history);
    if (nxt[a] != b) swap(a, b);
    if (nxt[a] == b) {
      nxt[a] = x, nxt[x] = b, pre[b] = x, pre[x] = a;
    } else {
      return {};
    }
  }

  vector<int> V = {0};
  For(N - 1) {
    V.emplace_back(nxt[V.back()]);
    if (V.back() == 0) return {};
  }
  return V;
}
} // namespace mitsuha
#endif // AJAY_OUTERPLANAR
