#define PROBLEM "https://yukicoder.me/problems/no/2296"
#include "library/template.hpp"
#include "library/graph/datastructure/link_cut_tree.hpp"
#include "library/graph/datastructure/lct_node_commutative_monoid.hpp"
#include "library/monoid/monoid_add.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"

using Node = LCT_Node_Commutative_Monoid<Monoid_Add<long long>, false>;

void solve() {
  LL(N, X, Q);

  long long nxt = N;
  UnionFind uf(N + N - 1);
  // root ごとに、直径頂点番号
  vector<pair<int, int>> dat(N + N - 1);
  For(i, N + N - 1) dat[i] = {i, i};

  Link_Cut_Tree<Node> tree(N + N - 1);

  auto add = [&](pair<int, int> p, int c) -> pair<int, int> {
    auto [a, b] = p;
    long long ab = tree.prod_path(a, b);
    long long ac = tree.prod_path(a, c);
    long long bc = tree.prod_path(b, c);
    long long mx = max<long long>({ab, ac, bc});
    if (mx == ab) return {a, b};
    if (mx == ac) return {a, c};
    return {b, c};
  };

  For(Q) {
    LL(t);
    if (t == 1) {
      LL(v, val);
      tree.set(nxt, val);
      tree.link(X, nxt);
      tree.link(nxt, v);
      nxt++;
      int a = uf[v], b = uf[X];
      uf.merge(a, b);
      int r = uf[a];
      for (int c: {a, b}) {
        if (r == c) continue;
        dat[r] = add(dat[r], dat[c].first);
        dat[r] = add(dat[r], dat[c].second);
      }
    }
    if (t == 2) {
      LL(u, v);
      if (tree.get_root(u) != tree.get_root(v)) {
        print(-1);
      } else {
        long long d = tree.prod_path(u, v);
        print(d);
        X += d;
        X %= N;
      }
    }
    if (t == 3) {
      LL(v);
      auto [a, b] = dat[uf[v]];
      long long d = tree.prod_path(a, b);
      print(d);
    }
    if (t == 4) {
      LL(val);
      X += val;
      X %= N;
    }
  }
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
