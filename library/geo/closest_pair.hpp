#ifndef AJAY_CLOSEST_PAIR
#define AJAY_CLOSEST_PAIR

#include "library/geo/base.hpp"
#include "library/random/base.hpp"
#include "library/random/shuffle.hpp"
#include "library/datastructure/hashmap.hpp"
#include "library/random/hash_pair.hpp"

namespace mitsuha{
template <typename T>
pair<int, int> closest_pair(vector<Point<T>> points) {
  int N = len(points);
  assert(N >= 2);
  HashMap<int> MP(N);
  vector<int> I(N);
  iota(All(I), 0);
  shuffle(I);
  points = rearrange(points, I);

  auto calc = [&](int i, int j) -> T {
    return (points[j] - points[i]).dot(points[j] - points[i]);
  };

  T best = calc(0, 1);
  pair<int, int> res = {0, 1};
  T w = sqrtl(best);

  vector<int> nxt(N, -1);

  auto insert = [&](int i) -> void {
    uint64_t k = hash_pair<long long>({points[i].x / w, points[i].y / w});
    nxt[i] = MP.get(k, -1);
    MP[k] = i;
  };

  auto query = [&](int i) -> bool {
    long long a = points[i].x / w;
    long long b = points[i].y / w;
    bool upd = 0;
    For(dx, -1, 2) For(dy, -1, 2) {
      uint64_t k = hash_pair<long long>({a + dx, b + dy});
      int j = MP.get(k, -1);
      while (j != -1) {
        if (chmin(best, calc(i, j))) { upd = 1, res = {i, j}, w = sqrtl(best); }
        j = nxt[j];
      }
    }
    return upd;
  };

  if (best == T(0)) {
    res.first = I[res.first], res.second = I[res.second];
    return res;
  }

  insert(0), insert(1);
  For(i, 2, N) {
    if (query(i)) {
      if (best == T(0)) break;
      MP.build(N);
      For(j, i) insert(j);
    }
    insert(i);
  }
  res.first = I[res.first], res.second = I[res.second];
  return res;
}

pair<int, int> closest_pair_dc(vector<Point<long long>> point) {
  int N = len(point);
  assert(N >= 2);

  auto I = argsort(point);
  point = rearrange(point, I);

  long long best = -1;
  pair<int, int> best_pair = {-1, -1};

  auto upd = [&](int i, int j) -> void {
    Point<long long> p = point[i] - point[j];
    long long d = p.dot(p);
    if (best == -1 || best > d) { best = d, best_pair = {I[i], I[j]}; }
  };
  upd(0, 1);

  auto dfs = [&](auto &dfs, int L, int R) -> vector<int> {
    // return: [L,R) sorted by y
    if (R == L + 1) return {L};
    int M = (L + R) / 2;
    vector<int> I0 = dfs(dfs, L, M);
    vector<int> I1 = dfs(dfs, M, R);
    vector<int> I;
    vector<int> near;
    int a = 0, b = 0;
    For(R - L) {
      int idx = [&]() -> int {
        if (a == len(I0)) return I1[b++];
        if (b == len(I1)) return I0[a++];
        int i = I0[a], j = I1[b];
        if (point[i].y < point[j].y) {
          ++a;
          return i;
        }
        ++b;
        return j;
      }();
      I.emplace_back(idx);
      long long dx = point[M].x - point[idx].x;
      if (dx * dx > best) { continue; }
      Frr(k, len(near)) {
        int j = near[k];
        long long dy = point[idx].y - point[j].y;
        if (best == 0 || dy * dy > best) break;
        upd(idx, j);
      }
      near.emplace_back(idx);
    }
    return I;
  };
  dfs(dfs, 0, N);
  return best_pair;
}

} // namespace mitsuha
#endif // AJAY_CLOSEST_PAIR
