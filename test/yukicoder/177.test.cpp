#define PROBLEM "https://yukicoder.me/problems/no/177"
#include "library/template.hpp"
#include "library/flow/maxflow.hpp"

void solve() {
  LL(W);
  LL(N);
  vector<long long> A(N); read(A);
  LL(M);
  vector<long long> C(M); read(C);
  long long source = N + M;
  long long sink = N + M + 1;
  MaxFlow<long long> G(N + M + 2, source, sink);
  auto left = [&](int i) -> int { return i; };
  auto right = [&](int i) -> int { return N + i; };
  For(i, N) G.add(source, left(i), A[i]);
  For(i, M) G.add(right(i), sink, C[i]);
  For(j, M) {
    LL(n);
    vector<long long> ok(N, 1);
    For(n) {
      LL(x);
      ok[--x] = 0;
    }
    For(i, N) if (ok[i]) { G.add(left(i), right(j), infty<long long>); }
  }
  long long f = G.flow();
  if (f < W)
    print("BANSAKUTSUKITA");
  else
    print("SHIROBAKO");
}

signed main() {
  solve();
  return 0;
}
