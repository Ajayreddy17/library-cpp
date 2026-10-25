#ifndef AJAY_RANGE_CLOSEST_PAIR_QUERY
#define AJAY_RANGE_CLOSEST_PAIR_QUERY

#include "library/datastructure/hashmap.hpp"
#include "library/datastructure/segment_tree/dual_segtree.hpp"
#include "library/monoid/monoid_min.hpp"

// Closest pair query for point set {p_i | i in [l, r)}
// O(KNlogKN + QlogN)
// https://qoj.ac/problem/5463
// https://codeforces.com/gym/104172/attachments/download/18933/Hong_Kong_Tutorial.pdf
// 1D point set: https://codeforces.com/problemset/problem/765/F
namespace mitsuha{
struct Range_Closest_Pair_Query {
  /*
  - Manage the answer per L while increasing R
  - L with 2^{k-1} <= ANS[L:R] < 2^{k}: level k (level 0: distance 0)
  - Prepare grids for levels 0, 1, 2, ..., 29
    - Width is 2^k
    - In a square of side 1.99, pairwise distance >= 1 -> at most 8 points
  - Search in order level 29, 28, ..., 0: 9 neighbors
    - When an answer is found, decrease the level. Propagate leftward.
    - Level decreases happen at most 30N times
  */
  const int LOG = 30;
  vector<pair<int, int>> point;
  vector<pair<int, int>> query;
  void add_point(int x, int y) {
    assert(0 <= x && x < (1 << LOG));
    assert(0 <= y && y < (1 << LOG));
    point.emplace_back(x, y);
  }
  void add_query(int L, int R) {
    assert(R - L >= 2);
    query.emplace_back(L, R);
  }
  long long dist(int i, int j) {
    long long dx = point[i].first - point[j].first;
    long long dy = point[i].second - point[j].second;
    return dx * dx + dy * dy;
  }

  vector<long long> calc() {
    const int K = LOG;
    const int N = len(point), Q = len(query);
    using A9 = array<int, 9>;
    // Cell index for each level
    vector<vector<int>> IDX(K, vector<int>(N, -1));
    // Neighbors for each cell index
    vector<A9> nbd;
    For(k, 1, K) {
      HashMap<int> MP(N);
      auto to_64 = [&](int x, int y) -> uint64_t { return uint64_t(x) << 30 | y; };
      int off = len(nbd);
      int p = off;
      For(i, N) {
        int x = point[i].first >> (k);
        int y = point[i].second >> (k);
        uint64_t key = to_64(x, y);
        if (!MP.contains(key)) {
          MP[key] = p++;
        }
        IDX[k][i] = MP[key];
      }
      nbd.resize(p);
      For(i, N) {
        int x = point[i].first >> (k);
        int y = point[i].second >> (k);
        int me = MP[to_64(x, y)];
        int s = 0;
        For(dx, -1, 2) For(dy, -1, 2) {
          uint64_t key = to_64(x + dx, y + dy);
          nbd[me][s++] = MP.get(key, -1);
        }
      }
    }

    vector<array<int, 8>> dat(len(nbd), {-1, -1, -1, -1, -1, -1, -1, -1});
    auto add = [&](int k, int i) -> void {
      int idx = IDX[k][i];
      for (auto&& j : dat[idx]) {
        if (j == -1) {
          j = i;
          return;
        }
      }
    };
    auto rm = [&](int k, int i) -> void {
      int idx = IDX[k][i];
      for (auto&& j : dat[idx]) {
        if (j == i) {
          j = -1;
          return;
        }
      }
    };

    auto solve_level = [&](int k, int i) -> vector<pair<int, long long>> {
      // Compute answers for level k point set
      vector<pair<int, long long>> res;
      int me = IDX[k][i];
      for (auto&& idx : nbd[me]) {
        if (idx == -1) continue;
        for (auto&& j : dat[idx]) {
          if (j == -1) continue;
          res.emplace_back(j, dist(i, j));
        }
      }
      return res;
    };
    Dual_SegTree<Monoid_Min<long long>> seg(N);
    vector<int> LEVEL(N, -1);
    auto get_lv = [&](long long d) -> int {
      if (d == 0) return 0;
      return (63 - __builtin_clzll(d)) / 2 + 1;
    };

    vector<int> left(Q);
    vector<vector<int>> query_at(N);
    For(qid, Q) {
      auto [L, R] = query[qid];
      left[qid] = L;
      query_at[--R].emplace_back(qid);
    }

    vector<long long> ANS(Q);

    For(R, N) {
      // Update answers using the R-th point
      vector<pair<int, long long>> upd;
      For(k, 1, K) {
        auto res = solve_level(k, R);
        upd.insert(upd.end(), All(res));
      }

      for (auto [i, d] : upd) {
        int lv = get_lv(d);
        if (seg.get(i) < d) continue;
        // Update the answer
        seg.apply(0, i + 1, d);
        // Update the level
        while (i >= 0 && LEVEL[i] > lv) {
          rm(LEVEL[i], i);
          LEVEL[i] = lv;
          if (lv) add(lv, i);
          --i;
        }
      }
      LEVEL[R] = K - 1;
      add(K - 1, R);
      for (auto&& qid : query_at[R]) {
        ANS[qid] = seg.get(left[qid]);
      }
    }
    return ANS;
  }
};

} // namespace mitsuha
#endif // AJAY_RANGE_CLOSEST_PAIR_QUERY
