#ifndef AJAY_COUNT_POINTS_IN_TRIANGLES
#define AJAY_COUNT_POINTS_IN_TRIANGLES


#include "library/geo/angle_sort.hpp"
#include "library/geo/base.hpp"
#include "library/random/base.hpp"
#include "library/datastructure/fenwick_tree/fenwicktree_01.hpp"

// Input point sets A, B (Point<long long>)
// query(i,j,k): returns count (non-negative) of B[l] inside triangle A[i]A[j]A[k]
// Preprocessing O(NMlogM), Query O(1)
// https://codeforces.com/contest/13/problem/D
// https://codeforces.com/contest/852/problem/H
namespace mitsuha{
struct Count_Points_in_Triangles {
  using P = Point<long long>;
  const int LIM = 1'000'000'000 + 10;
  vector<P> A, B;
  vector<int> new_idx; // manages argument-sorted order as seen from O
  vector<int> point;   // count of B[j] coinciding with A[i]
  vector<vector<int>> seg;    // count of B[k] on segment A[i]A[j]
  vector<vector<int>> tri;    // count of B[k] inside triangle OA[i]A[j]
  Count_Points_in_Triangles(const vector<P>& A, const vector<P>& B) : A(A), B(B) {
    for (auto&& p: A) assert(max(abs(p.x), abs(p.y)) < LIM);
    for (auto&& p: B) assert(max(abs(p.x), abs(p.y)) < LIM);
    build();
  }

  int count3(int i, int j, int k) {
    i = new_idx[i], j = new_idx[j], k = new_idx[k];
    if (i > j) swap(i, j);
    if (j > k) swap(j, k);
    if (i > j) swap(i, j);
    assert(i <= j && j <= k);
    long long d = (A[j] - A[i]).det(A[k] - A[i]);
    if (d == 0) return 0;
    if (d > 0) { return tri[i][j] + tri[j][k] - tri[i][k] - seg[i][k]; }
    int x = tri[i][k] - tri[i][j] - tri[j][k];
    return x - seg[i][j] - seg[j][k] - point[j];
  }

  // segment
  int count2(int i, int j) {
    i = new_idx[i], j = new_idx[j];
    if (i > j) swap(i, j);
    return seg[i][j];
  }

private:
  P take_origin() {
    // Ensure OAiAj, OAiBj are not collinear
    // fail prob: at most N(N+M)/LIM
    return P{-LIM, RNG(-LIM, LIM)};
  }

  void build() {
    P O = take_origin();
    for (auto&& p: A) p = p - O;
    for (auto&& p: B) p = p - O;
    int N = len(A), M = len(B);
    vector<int> I = angle_sort(A);
    A = rearrange(A, I);
    new_idx.resize(N);
    For(i, N) new_idx[I[i]] = i;

    I = angle_sort(B);
    B = rearrange(B, I);

    point.assign(N, 0);
    seg.assign(N, vector<int>(N));
    tri.assign(N, vector<int>(N));

    // point
    For(i, N) For(j, M) if (A[i] == B[j])++ point[i];

    int m = 0;
    For(j, N) {
      // OA[i]A[j], B[k]
      while (m < M && A[j].det(B[m]) < 0) ++m;
      vector<P> C(m);
      For(k, m) C[k] = B[k] - A[j];
      vector<int> I(m);
      For(i, m) I[i] = i;
      sort(All(I), [&](auto& a, auto& b) -> bool { return C[a].det(C[b]) > 0; });
      C = rearrange(C, I);
      vector<int> rk(m);
      For(k, m) rk[I[k]] = k;
      FenwickTree_01 bit(m);

      int k = m;
      Frr(i, j) {
        while (k > 0 && A[i].det(B[k - 1]) > 0) { bit.add(rk[--k], 1); }
        P p = A[i] - A[j];
        int lb = [&]() { int lo = 0, hi = m + 1; while (lo < hi) { int mid = (lo + hi) / 2; if (mid == 0 ? true : C[mid - 1].det(p) > 0) lo = mid + 1; else hi = mid; } return lo - 1; }();
        int ub = [&]() { int lo = 0, hi = m + 1; while (lo < hi) { int mid = (lo + hi) / 2; if (mid == 0 ? true : C[mid - 1].det(p) >= 0) lo = mid + 1; else hi = mid; } return lo - 1; }();
        seg[i][j] += bit.sum(lb, ub), tri[i][j] += bit.sum(lb);
      }
    }
  }
};

} // namespace mitsuha
#endif // AJAY_COUNT_POINTS_IN_TRIANGLES
