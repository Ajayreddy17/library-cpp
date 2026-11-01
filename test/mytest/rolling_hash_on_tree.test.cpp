#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/datastructure/rolling_hash_on_tree.hpp"
#include "library/random/random_graph.hpp"
#include "library/util/maspy_compat.hpp"

void test_edge() {
  long long N = RNG(1, 20);
  auto edges = random_tree(N);
  Graph<int, 0> G(N);
  for (auto& [a, b] : edges) {
    int x = RNG(0, 3);
    G.add(a, b, x);
  }
  G.build();
  Tree<decltype(G)> tree(G);

  using mint = modint61;
  mint base = RNG_64();

  Rolling_Hash_on_Tree<decltype(tree), true> RH(
      tree, [&](int i) -> int { return G.edges[i].cost; }, base);

  vector<vector<vector<int>>> dat(N, vector<vector<int>>(N, vector<int>(0)));
  For(a, N) For(b, N) {
    vector<int> P = tree.restore_path(a, b);
    vector<int> S;
    For(i, len(P) - 1) {
      int eid = tree.get_eid(P[i], P[i + 1]);
      S.emplace_back(G.edges[eid].cost);
    }
    dat[a][b] = S;
  }

  For(a, N) For(b, N) {
    mint h = 0;
    for (auto& x : dat[a][b]) {
      h = h * base + x;
    }
    assert(h == RH.get(a, b));
  }
  For(a, N) For(b, N) For(c, N) For(d, N) {
    vector<int> A = dat[a][b], B = dat[c][d];
    int lcp = 0;
    while (lcp < len(A) && lcp < len(B) && A[lcp] == B[lcp]) ++lcp;
    auto [k, ch] = RH.lcp_and_comp(a, b, c, d);
    assert(k == lcp);
    if (ch == '<') assert(A < B);
    if (ch == '=') assert(A == B);
    if (ch == '>') assert(A > B);
  }
}

void test_vertex() {
  long long N = RNG(1, 20);
  auto edges = random_tree(N);
  Graph<int, 0> G(N);
  for (auto& [a, b] : edges) {
    G.add(a, b);
  }
  vector<int> A(N);
  For(i, N) A[i] = RNG(0, 3);
  G.build();
  Tree<decltype(G)> tree(G);

  using mint = modint61;
  mint base = RNG_64();

  Rolling_Hash_on_Tree<decltype(tree), false> RH(
      tree, [&](int i) -> int { return A[i]; }, base);

  vector<vector<vector<int>>> dat(N, vector<vector<int>>(N, vector<int>(0)));
  For(a, N) For(b, N) {
    vector<int> P = tree.restore_path(a, b);
    vector<int> S;
    for (auto& v : P) S.emplace_back(A[v]);
    dat[a][b] = S;
  }

  For(a, N) For(b, N) {
    mint h = 0;
    for (auto& x : dat[a][b]) {
      h = h * base + x;
    }
    assert(h == RH.get(a, b));
  }

  For(a, N) For(b, N) For(c, N) For(d, N) {
    vector<int> A = dat[a][b], B = dat[c][d];
    int lcp = 0;
    while (lcp < len(A) && lcp < len(B) && A[lcp] == B[lcp]) ++lcp;
    auto [k, ch] = RH.lcp_and_comp(a, b, c, d);
    assert(k == lcp);
    if (ch == '<') assert(A < B);
    if (ch == '=') assert(A == B);
    if (ch == '>') assert(A > B);
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  For(300) test_edge();
  For(300) test_vertex();
  solve();
}
