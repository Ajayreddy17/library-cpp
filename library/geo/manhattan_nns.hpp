#ifndef AJAY_MANHATTAN_NNS
#define AJAY_MANHATTAN_NNS

#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/monoid/monoid_minidx.hpp"

// 点群 FRM から点群 TO への最近点探索
// vector の pair を返す：dist, nbd_idx
namespace mitsuha{
template <typename X = long long>
pair<vector<X>, vector<int>> manhattan_nns(vector<pair<X, X>> FRM, vector<pair<X, X>>& TO) {
  assert(len(TO) >= 1);
  int N = len(FRM), M = len(TO);
  vector<pair<X, X>> points(N + M);
  For(i, N) points[i] = FRM[i];
  For(i, M) points[N + i] = TO[i];
  vector<X> Y(M);
  For(i, M) Y[i] = TO[i].second;
  settify(Y);

  vector<int> nbd_idx(N, -1);
  vector<X> dist(N, infty<X>);

  auto add_ans = [&](int i, int j) -> void {
    if (j == -1) return;
    X dx = points[i].first - points[j].first;
    X dy = points[i].second - points[j].second;
    if (chmin(dist[i], abs(dx) + abs(dy))) nbd_idx[i] = j - N;
  };

  vector<int> I(N + M);
  iota(All(I), 0);
  sort(All(I), [&](auto& i, auto& j) { return (points[i].first < points[j].first); });
  auto calc = [&]() -> void {
    SegTree<Monoid_Min_Idx<X>> seg1(len(Y)), seg2(len(Y));
    for (auto&& i : I) {
      auto [x, y] = points[i];
      int idx = (int)(lower_bound(Y.begin(), Y.end(), y) - Y.begin());
      if (i < N) {
        add_ans(i, seg1.prod(idx, len(Y)).second);
        add_ans(i, seg2.prod(0, idx).second);
      }
      elif (i >= N) {
        seg1.set(idx, {y - x, i});
        seg2.set(idx, {-(x + y), i});
      }
    }
  };

  calc();
  reverse(All(I));
  For(i, N + M) points[i].first *= -1;
  calc();

  return {dist, nbd_idx};
};

} // namespace mitsuha
#endif // AJAY_MANHATTAN_NNS
