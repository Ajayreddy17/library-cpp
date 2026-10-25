#ifndef AJAY_MIN_SLOPE_SUM
#define AJAY_MIN_SLOPE_SUM


// https://uoj.ac/contest/104/problem/1037
// A, B form lower convex hulls and A[i]+B[j] is guaranteed to have positive x-coordinate
// Find the min slope of A[i]+B[j]. O(log N+log M)
namespace mitsuha{
template <typename P>
P min_slope_sum(vector<P>& A, vector<P>& B) {
  int N = len(A), M = len(B);
  long long NL = 0, NR = N;
  long long ML = 0, MR = M;

  auto better = [&](P p, P q) -> bool { return p.y * q.x < q.y * p.x; };

  while (NL + 1 < NR && ML + 1 < MR) {
    long long n = (NL + NR) / 2;
    long long m = (ML + MR) / 2;
    P p = A[n] + B[m];
    P pa = A[n] - A[n - 1];
    P pb = B[m] - B[m - 1];
    if (better(pa, pb)) {
      if (better(pb, p)) {
        NL = n;
      } else {
        MR = m;
      }
    } else {
      if (better(pa, p)) {
        ML = m;
      } else {
        NR = n;
      }
    }
  }
  while (ML + 1 < MR) {
    long long n = NL, m = (ML + MR) / 2;
    P p = A[n] + B[m];
    (better(p, A[n] + B[m - 1]) ? ML : MR) = m;
  }
  while (NL + 1 < NR) {
    long long m = ML, n = (NL + NR) / 2;
    P p = A[n] + B[m];
    (better(p, A[n - 1] + B[m]) ? NL : NR) = n;
  }
  return A[NL] + B[ML];
}

} // namespace mitsuha
#endif // AJAY_MIN_SLOPE_SUM
