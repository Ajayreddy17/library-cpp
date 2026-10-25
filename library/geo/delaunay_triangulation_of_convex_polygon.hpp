#ifndef AJAY_DELAUNAY_TRIANGULATION_OF_CONVEX_POLYGON
#define AJAY_DELAUNAY_TRIANGULATION_OF_CONVEX_POLYGON

#include "library/geo/outcircle.hpp"
#include "library/datastructure/fastset.hpp"
#include "library/random/shuffle.hpp"

// 座標の 4 乗がオーバーフローしない
// return : array<int,3>, triangles
// https://codeforces.com/contest/154/problem/E
// https://codeforces.com/contest/549/problem/E
// https://codeforces.com/contest/1984/problem/H
namespace mitsuha{
template <typename T>
vector<array<int, 3>> delaunay_triangulation_of_convex_polygon(vector<Point<T>> A, bool farthest) {
  int N = len(A);
  if (N <= 2) return {};

  FastSet FS(N);

  vector<int> I(N);
  For(i, N) I[i] = i;
  shuffle(I);

  sort(I.end() - 3, I.end());

  struct E {
    int a, b, nxt, rev;
  };

  auto pop = [](auto& v) { auto x = v.back(); v.pop_back(); return x; };
  int c = pop(I), b = pop(I), a = pop(I);
  vector<E> dat;
  dat.emplace_back(a, b, 1, -1);
  dat.emplace_back(b, c, 2, -1);
  dat.emplace_back(c, a, 0, -1);
  vector<int> v_to_e(N, -1);
  v_to_e[a] = 0, v_to_e[b] = 1, v_to_e[c] = 2;
  FS.insert(a), FS.insert(b), FS.insert(c);

  auto dfs = [&](auto& dfs, int i) -> void {
    int j = dat[i].rev;
    if (j == -1) return;
    int i1 = dat[i].nxt;
    int i2 = dat[i1].nxt;
    int j1 = dat[j].nxt;
    int j2 = dat[j1].nxt;
    int b = dat[i].a, c = dat[i1].a, a = dat[i2].a, d = dat[j2].a;
    int side = outcircle_side(A[a], A[b], A[c], A[d]);
    bool flip = (farthest ? (side == -1) : (side == 1));
    if (!flip) return;
    dat[i] = {d, a, i2, j};
    dat[j] = {a, d, j2, i};
    dat[i1].nxt = j, dat[i2].nxt = j1, dat[j1].nxt = i, dat[j2].nxt = i1;
    dfs(dfs, i1), dfs(dfs, i2), dfs(dfs, j1), dfs(dfs, j2);
  };

  while (len(I)) {
    int v = pop(I);
    int l = FS.prev(v), r = FS.next(v);
    if (l == -1) l = FS.prev(N);
    if (r == N) r = FS.next(0);
    FS.insert(v);
    int k = v_to_e[l];
    int s = len(dat);
    v_to_e[l] = s + 1, v_to_e[v] = s + 2;
    dat[k].rev = s;
    dat.emplace_back(r, l, s + 1, k);
    dat.emplace_back(l, v, s + 2, -1);
    dat.emplace_back(v, r, s, -1);
    dfs(dfs, k);
  }
  vector<array<int, 3>> ANS;
  For(i, len(dat)) {
    int j = dat[i].nxt;
    int k = dat[j].nxt;
    if (i > j || i > k) continue;
    ANS.emplace_back(array<int, 3>{dat[i].a, dat[j].a, dat[k].a});
  }

  return ANS;
}

} // namespace mitsuha
#endif // AJAY_DELAUNAY_TRIANGULATION_OF_CONVEX_POLYGON
