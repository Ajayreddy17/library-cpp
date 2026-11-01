#ifndef AJAY_UNLABELED_TREE
#define AJAY_UNLABELED_TREE

#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/enumerate/unlabeled_tree.hpp

// enumerate unlabeled unrooted trees up to isomorphism, as vector<pair<int, int>>
// https://oeis.org/A000055
// N=15: 7741, 0.005sec
// N=20: 823065, 0.401sec
// N=25: 104636890, 52.368sec
template <typename F>
void enumerate_unlabeled_tree(int n, F f) {
  assert(0 <= n && n <= 32);
  if (n == 0) return;
  /*
  enumerate rooted trees with at most n/2 vertices
  rooted tree -> bracket sequence -> 1/0 sequence -> 32-bit integer
  */
  int m = fld<int>(n, 2);
  vector<vector<uint32_t>> dat(m + 1);
  if (1 <= m) dat[1].emplace_back(0);

  For(n, 2, m + 1) {
    auto dfs = [&](auto& dfs, int m, int k, uint32_t now, int now_e) -> void {
      if (now_e == n - 1) {
        dat[n].emplace_back(now);
        return;
      }
      if (now_e + m >= n) {
        m = n - 1 - now_e;
        k = 0;
      }
      if (m == 0) return;
      For(i, k, len(dat[m])) {
        uint32_t x = dat[m][i];
        x = (x << 1) | 1;
        dfs(dfs, m, i, now | x << (2 * now_e), now_e + m);
      }
      dfs(dfs, m - 1, 0, now, now_e);
    };
    dfs(dfs, n - 1, 0, 0, 0);
  }

  // rooted trees of size <= m are enumerated; use them to
  // enumerate unrooted trees with n vertices
  auto decode = [&](uint64_t x) -> vector<pair<int, int>> {
    vector<pair<int, int>> edge;
    vector<int> path = {0};
    int p = 0;
    For(i, 2 * n - 2) {
      if (x >> i & 1) {
        edge.emplace_back(path.back(), p + 1);
        path.emplace_back(p + 1), p++;
      } else {
        path.pop_back();
      }
    }
    return edge;
  };

  auto dfs = [&](auto& dfs, int m, int k, uint64_t now, int now_e) -> void {
    if (now_e == n - 1) {
      f(decode(now));
      return;
    }
    if (now_e + m >= n) {
      m = n - 1 - now_e;
      k = 0;
    }
    if (m == 0) return;
    For(i, k, len(dat[m])) {
      uint64_t x = dat[m][i];
      x = (x << 1) | 1;
      dfs(dfs, m, i, now | x << (2 * now_e), now_e + m);
    }
    dfs(dfs, m - 1, 0, now, now_e);
  };
  dfs(dfs, fld<int>(n - 1, 2), 0, 0, 0);

  // two centroids
  if (2 * m == n) {
    For(i, len(dat[m])) {
      For(j, i + 1) {
        uint64_t x = dat[m][i], y = dat[m][j];
        y = (y << 1) | 1;
        f(decode(x | (y << (2 * (m - 1)))));
      }
    }
  }
}

template <typename F>
void enumerate_unlabeled_rooted_tree(int n, F f) {
  assert(0 <= n && n <= 32);
  if (n == 0) return;
  /*
  enumerate rooted trees with at most n/2 vertices
  rooted tree -> bracket sequence -> 1/0 sequence -> 32-bit integer
  */
  int m = n;
  vector<vector<uint32_t>> dat(n + 1);
  if (1 <= m) dat[1].emplace_back(0);

  For(n, 2, m + 1) {
    auto dfs = [&](auto& dfs, int m, int k, uint32_t now, int now_e) -> void {
      if (now_e == n - 1) {
        dat[n].emplace_back(now);
        return;
      }
      if (now_e + m >= n) {
        m = n - 1 - now_e;
        k = 0;
      }
      if (m == 0) return;
      For(i, k, len(dat[m])) {
        uint32_t x = dat[m][i];
        x = (x << 1) | 1;
        dfs(dfs, m, i, now | x << (2 * now_e), now_e + m);
      }
      dfs(dfs, m - 1, 0, now, now_e);
    };
    dfs(dfs, n - 1, 0, 0, 0);
  }

  // rooted trees of size <= m are enumerated; use them to
  // enumerate unrooted trees with n vertices
  auto decode = [&](uint64_t x) -> vector<pair<int, int>> {
    vector<pair<int, int>> edge;
    vector<int> path = {0};
    int p = 0;
    For(i, 2 * n - 2) {
      if (x >> i & 1) {
        edge.emplace_back(path.back(), p + 1);
        path.emplace_back(p + 1), p++;
      } else {
        path.pop_back();
      }
    }
    return edge;
  };

  for (auto& x : dat[n]) f(decode(x));
}
} // namespace mitsuha
#endif // AJAY_UNLABELED_TREE
