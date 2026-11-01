#ifndef AJAY_COUNT_C3_C4
#define AJAY_COUNT_C3_C4

#include "library/graph/base.hpp"
#include "library/util/maspy_compat.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/count/count_C3_C4.hpp

// for each vertex, count C3 and C4 containing it
// assumes a simple graph
template <typename GT>
pair<vector<long long>, vector<long long>> count_C3_C4_pointwise(GT &G) {
  static_assert(!GT::is_directed);
  int N = G.N;
  auto deg = G.deg_array();
  auto I = argsort(deg);
  reverse(All(I));
  vector<int> rk(N);
  For(i, N) rk[I[i]] = i;

  // sort successors in decreasing order
  vector<vector<int>> TO(N);
  for (auto &&e: G.edges) {
    int a = rk[e.frm], b = rk[e.to];
    TO[a].emplace_back(b), TO[b].emplace_back(a);
  }
  For(v, N) { sort(All(TO[v])), reverse(All(TO[v])); }

  vector<int> A(N);
  vector<long long> C3(N), C4(N);
  For(a, N) {
    for (auto &b: TO[a]) TO[b].pop_back();
    for (auto &b: TO[a]) {
      for (auto &c: TO[b]) { C4[a] += A[c], C4[c] += A[c], A[c] += 1; }
    }
    for (auto &b: TO[a]) {
      C3[a] += A[b], C3[b] += A[b] + A[b];
      for (auto &c: TO[b]) { C4[b] += A[c] - 1; }
    }
    for (auto &b: TO[a]) {
      for (auto &c: TO[b]) { A[c] = 0; }
    }
  }
  for (auto &x: C3) x /= 2;
  C3 = rearrange(C3, rk), C4 = rearrange(C4, rk);
  return {C3, C4};
}

// 500 ms for (2e5, 5e5)
// https://codeforces.com/gym/104053/problem/K
template <typename GT>
pair<long long, long long> count_C3_C4(GT &G) {
  static_assert(!GT::is_directed);
  int N = G.N;
  long long x3 = 0, x4 = 0;
  auto deg = G.deg_array();
  auto I = argsort(deg);
  reverse(All(I));
  vector<int> rk(N);
  For(i, N) rk[I[i]] = i;

  // sort successors in decreasing order
  vector<vector<int>> TO(N);
  for (auto &&e: G.edges) {
    int a = rk[e.frm], b = rk[e.to];
    if (a != b) TO[a].emplace_back(b), TO[b].emplace_back(a);
  }
  For(v, N) {
    sort(All(TO[v]));
    reverse(All(TO[v]));
  }

  vector<int> A(N);
  For(a, N) {
    for (auto &&b: TO[a]) TO[b].pop_back();
    for (auto &&b: TO[a]) {
      for (auto &&c: TO[b]) { x4 += A[c]++; }
    }
    for (auto &&b: TO[a]) { x3 += A[b]; }
    for (auto &&b: TO[a]) {
      for (auto &&c: TO[b]) { A[c] = 0; }
    }
  }
  x3 /= 2;
  return {x3, x4};
}
} // namespace mitsuha
#endif // AJAY_COUNT_C3_C4
