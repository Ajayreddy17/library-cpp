#define PROBLEM "https://yukicoder.me/problems/no/2338"
#include "library/template.hpp"
#include "library/datastructure/offline_query/rectangle_add_point_sum.hpp"

void solve() {
  LL(N, M, Q);
  vector<int> P(N);
  vector<int> S(N);
  For(i, N) {
    read(P[i]);
    --P[i];
    STR(x);
    S[i] = (x == "AC" ? 1 : 0);
  }

  vector<vector<int>> IDX(M);
  For(i, N) { IDX[P[i]].emplace_back(i); }

  Rectangle_Add_Point_Sum<Monoid_Add<int>, int, true> AC, PENA;

  For(m, M) {
    auto I = IDX[m];
    int n = len(I);

    vector<int> NXT(n, N), PRE(n, -1);
    For(i, n) {
      if (i) chmax(PRE[i], PRE[i - 1]);
      if (S[I[i]]) PRE[i] = I[i];
    }

    int nxt_AC = N;
    Frr(i, len(I)) {
      int idx = I[i];
      int t = S[idx];
      if (t == 0) {
        // WA
        int a = PRE[i];
        int b = nxt_AC;
        if (b == N) continue;
        PENA.add_query(a + 1, idx + 1, b + 1, N + 1, 1);
      }
      if (t == 1) {
        // AC
        AC.add_query(0, idx + 1, idx + 1, nxt_AC + 1, 1);
        // print("AC", 0, idx + 1, idx + 1, nxt_AC + 1);
        nxt_AC = idx;
      }
    }
  }

  For(Q) {
    LL(L, R);
    --L;
    AC.sum_query(L, R);
    PENA.sum_query(L, R);
  }
  vector<int> ANS1 = AC.calc();
  vector<int> ANS2 = PENA.calc();
  For(i, Q) { print(ANS1[i], ANS2[i]); }
}

signed main() {
  int T = 1;
  // INT(T);
  For(T) solve();
  return 0;
}
