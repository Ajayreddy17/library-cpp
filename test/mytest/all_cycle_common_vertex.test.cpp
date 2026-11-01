#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/graph/base.hpp"
#include "library/graph/all_cycle_common_vertices.hpp"
#include "library/random/base.hpp"

// #include "other/io.hpp"



vector<int> naive(Graph<int, 1> G) {
  int N = G.N;
  vector<int> ANS;
  auto dag = [&](vector<int> V) -> bool {
    Graph<int, 1> H = G.rearrange(V);
    return V.empty() || (!toposort(H).empty());
  };
  vector<int> V;
  For(i, N) V.emplace_back(i);
  if (dag(V)) return {};
  For(v, N) {
    V.erase(find(V.begin(), V.end(), v));
    if (dag(V)) ANS.emplace_back(v);
    V.emplace_back(v);
  }
  return ANS;
}

void test() {
  For(1 << 18) {
    int N = RNG(1, 4);
    Graph<int, 1> G(N);
    int prob = RNG(0, 100);
    For(a, N) For(b, N) {
      if (a == b) continue;
      if (RNG(0, 100) < prob) G.add(a, b);
    }
    G.build();
    auto ANS1 = all_cycle_common_vertices(G, 0);
    auto ANS2 = naive(G);
    sort(ANS1.begin(), ANS1.end());
    sort(ANS2.begin(), ANS2.end());
    // G.debug_print();
    // debug(ANS1, ANS2);
    assert(ANS1 == ANS2);
  }
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  test();
  solve();
}
