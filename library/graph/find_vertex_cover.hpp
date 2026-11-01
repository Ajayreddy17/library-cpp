#ifndef AJAY_FIND_VERTEX_COVER
#define AJAY_FIND_VERTEX_COVER


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/find_vertex_cover.hpp

// returns a vertex cover of size <= K if one exists
// about NK*1.618^K
// handling paths and cycles specially would make it faster
// the graph representation could be better
// https://codeforces.com/contest/164/problem/D
template <typename GT>
vector<int> find_vertex_cover(GT G, int K) {
  int N = G.N;
  vector<vector<bool>> adj(N, vector<bool>(N));
  for (auto& e: G.edges) {
    assert(e.frm != e.to);
    adj[e.frm][e.to] = adj[e.to][e.frm] = 1;
  }
  vector<int> deg(N);
  For(v, N) deg[v] = accumulate(adj[v].begin(), adj[v].end(), int(0));

  if (K == 0) return {};

  vector<bool> used(N);
  vector<int> S;
  bool end = 0;
  int M = accumulate(deg.begin(), deg.end(), int(0)) / 2;

  auto on = [&](int v) -> void {
    assert(!used[v]);
    M -= deg[v];
    For(w, N) {
      if (!used[w] && adj[v][w]) { deg[v]--, deg[w]--; }
    }
    assert(deg[v] == 0);
    used[v] = 1;
    S.emplace_back(v);
  };
  auto off = [&](int v) -> void {
    assert(S.back() == v);
    S.pop_back();
    used[v] = 0;
    For(w, N) {
      if (!used[w] && adj[v][w]) { deg[v]++, deg[w]++; }
    }
    M += deg[v];
  };

  auto dfs = [&](auto& dfs) -> void {
    if (end) return;
    int c = max_element(deg.begin(), deg.end()) - deg.begin();
    if (deg[c] * (K - len(S)) < M) { return; }
    if (deg[c] == 0) {
      assert(M == 0);
      end = 1;
      return;
    }
    if (len(S) == K) { return; }
    on(c);
    dfs(dfs);
    if (end) return;
    off(c);
    if (deg[c] == 1) return;
    if (len(S) + deg[c] > K) return;
    int k = len(S);
    For(v, N) {
      if (adj[v][c] && !used[v]) { on(v); }
    }
    dfs(dfs);
    if (end) return;
    while (len(S) > k) { off(S.back()); }
  };
  dfs(dfs);
  if (!end) assert(S.empty());
  return S;
}
} // namespace mitsuha
#endif // AJAY_FIND_VERTEX_COVER
