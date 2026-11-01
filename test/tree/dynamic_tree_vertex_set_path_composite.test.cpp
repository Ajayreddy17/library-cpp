#define PROBLEM  "https://judge.yosupo.jp/problem/dynamic_tree_vertex_set_path_composite"
#include "library/template.hpp"
#include "library/graph/datastructure/link_cut_tree.hpp"
#include "library/graph/datastructure/lct_node_monoid.hpp"
#include "library/monoid/monoid_affine.hpp"
#include "library/mod/modint.hpp"
#include "library/util/maspy_compat.hpp"

using mint = modint998;

using Mono = Monoid_Affine<mint>;
using Node = LCT_Node_Monoid<Mono>;

void solve() {
  LL(N, Q);
  Link_Cut_Tree<Node> LCT(N);
  For(i, N) {
    mint a, b;
    read(a, b);
    LCT.set(i, {a, b});
  }
  For(N - 1) {
    INT(a, b);
    LCT.link(a, b);
  }

  For(Q) {
    LL(t);
    if (t == 0) {
      LL(a, b, c, d);
      LCT.cut(a, b), LCT.link(c, d);
    }
    if (t == 1) {
      LL(i);
      mint a, b;
      read(a, b);
      LCT.set(i, {a, b});
    }
    if (t == 2) {
      LL(a, b);
      auto f = LCT.prod_path(a, b);
      uint32_t x;
      read(x);
      mint ans = Mono::eval(f, mint::raw(x));
      print(ans);
    }
  }
}

signed main() {
  solve();
  return 0;
}
