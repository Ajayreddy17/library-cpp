#define PROBLEM  "https://judge.yosupo.jp/problem/point_set_tree_path_composite_sum"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/datastructure/dynamic_rerooting_tree_dp.hpp"

using mint = modint998;

struct Data {
  // always define type, s, t (you will need them when debugging anyway). s is the root.
  // type==0: s is virtual. type==1: t is virtual.
  int type, s, t;
  mint a, b;  // path composition
  mint cnt, ans;
};

struct TREE_DP {
  using value_type = Data;
  using X = value_type;

  static X rake(const X& L, const X& R) {
    assert(L.type == 0 && R.type == 0 && L.s == R.s);
    X ANS = {0, L.s, L.t};

    ANS.a = L.a, ANS.b = L.b;
    ANS.cnt = L.cnt + R.cnt, ANS.ans = L.ans + R.ans;
    return ANS;
  }
  static X rake2(const X& L, const X& R) {
    assert(L.type == 1 && R.type == 0 && L.s == R.s);
    X ANS = {1, L.s, L.t};

    ANS.a = L.a, ANS.b = L.b;
    ANS.cnt = L.cnt + R.cnt, ANS.ans = L.ans + R.ans;
    return ANS;
  }
  static X compress(const X& L, const X& R) {
    assert(L.type == 0 && R.type == 0 && L.t == R.s);
    X ANS = {0, L.s, R.t};

    ANS.a = L.a * R.a, ANS.b = L.a * R.b + L.b;
    ANS.cnt = L.cnt + R.cnt, ANS.ans = L.ans + (L.a * R.ans + L.b * R.cnt);
    return ANS;
  }
  static X compress2(const X& L, const X& R) {
    assert(L.type == 1 && R.type == 1 && L.t == R.s);
    X ANS = {1, L.s, R.t};

    ANS.a = L.a * R.a, ANS.b = L.a * R.b + L.b;
    ANS.cnt = L.cnt + R.cnt, ANS.ans = L.ans + (L.a * R.ans + L.b * R.cnt);
    return ANS;
  }
  static X compress3(const X& L, const X& R) {
    assert(L.type == 1 && R.type == 0 && L.t == R.s);
    X ANS = {1, L.s, L.t};

    ANS.a = L.a, ANS.b = L.b;
    ANS.cnt = L.cnt + R.cnt, ANS.ans = L.ans + (L.a * R.ans + L.b * R.cnt);
    return ANS;
  }
};

void solve() {
  LL(N, Q);
  vector<mint> A(N); read(A);
  vector<mint> B(N - 1), C(N - 1);
  Graph<int, 0> G(N);
  For(i, N - 1) {
    INT(u, v);
    G.add(u, v);
    read(B[i], C[i]);
  }
  G.build();

  Tree<decltype(G)> tree(G);

  auto single = [&](int v) -> pair<Data, Data> {
    if (v == 0) {
      Data up = {0, -1, 0, 1, 0, 1, A[v]};
      Data down = {1, 0, -1, 1, 0, 1, A[v]};
      return {up, down};
    }
    int e = tree.v_to_e(v);
    int p = tree.parent[v];
    Data up = {0, p, v, B[e], C[e], 1, B[e] * A[v] + C[e]};
    Data down = {1, v, p, B[e], C[e], 1, A[v]};
    return {up, down};
  };

  Dynamic_Rerooting_Tree_DP<decltype(tree), TREE_DP> DP(tree, single);
  For(Q) {
    INT(t);
    if (t == 0) {
      INT(w, x);
      A[w] = x;
      DP.set(w, single(w));
    }
    if (t == 1) {
      INT(e, b, c);
      B[e] = b, C[e] = c;
      int v = tree.e_to_v(e);
      DP.set(v, single(v));
    }
    INT(r);
    Data x = DP.prod_all(r);
    print(x.ans);
  }
}

signed main() {
  solve();
  return 0;
}
