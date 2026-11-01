#define PROBLEM "https://yukicoder.me/problems/no/114"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/graph/steiner_tree.hpp"
#include "library/util/bit_utils.hpp"

void solve() {
  LL(N, M, K);
  Graph<int> G(N);
  G.read_graph(M, 1);
  vector<int> terminal(K); read(terminal);
  for (auto&& v: terminal) --v;

  if (K <= 15) return print(get<0>(steiner_tree<int>(G, terminal)));

  set<int> ss(terminal.begin(), terminal.end());
  vector<int> other;
  For(v, N) if (!ss.count(v)) other.emplace_back(v);

  UnionFind uf;
  long long LIM = 100;

  auto edges = G.edges;
  sort(edges.begin(), edges.end(), [](auto& x, auto& y) { return x.cost < y.cost; });

  auto f = [&](long long s) -> int {
    uf.build(N);
    int mst = 0, merge = 0;
    for (auto&& e: edges) {
      int i = e.frm, j = e.to;
      if ((s >> i & 1) && (s >> j & 1)) {
        if (uf.merge(i, j)) {
          ++merge;
          mst += e.cost;
        }
      }
    }
    if (merge == popcount(s) - 1) return mst;
    return infty<int>;
  };

  int ANS = infty<int>;
  long long t = 0;
  for (auto&& x: terminal) t |= 1LL << x;

  For(s, 1 << len(other)) {
    long long u = t;
    For(i, len(other)) if (s & 1 << i) u |= 1LL << other[i];
    chmin(ANS, f(u));
  }
  print(ANS);
}

signed main() {
  solve();
  return 0;
}
