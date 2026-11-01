#ifndef AJAY_TOURNAMENT
#define AJAY_TOURNAMENT


namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/graph/tournament.hpp
// https://yukicoder.me/problems/no/2085
// https://codeforces.com/problemset/problem/412/D
template <typename F>
vector<int> hamiltonian_path_in_tournament(int n, F check) {
  auto dfs = [&](auto& dfs, int L, int R) -> vector<int> {
    if (R == L + 1) return {L};
    int M = (L + R) / 2;
    vector<int> X = dfs(dfs, L, M);
    vector<int> Y = dfs(dfs, M, R);
    vector<int> P;
    P.reserve(R - L);
    int i = 0, j = 0;
    while (len(P) < R - L) {
      if (i == len(X)) {
        P.emplace_back(Y[j++]);
      }
      elif (j == len(Y)) { P.emplace_back(X[i++]); }
      else {
        if (check(X[i], Y[j])) {
          P.emplace_back(X[i++]);
        } else {
          P.emplace_back(Y[j++]);
        }
      }
    }
    return P;
  };
  return dfs(dfs, 0, n);
}

// https://atcoder.jp/contests/joisp2024/tasks/joisp2024_l
vector<string> from_outdegrees(vector<int> A) {
  int N = len(A);
  For(i, N - 1) assert(A[i] <= A[i + 1]);
  for (auto x : A) {
    if (x < 0 || x >= N) return {};
  }

  vector<int> F(N);
  for (auto x : A) F[x]++;

  vector<string> ANS(N, string(N, '-'));
  For(i, N) {
    int lose = N - 1 - i - A[i];
    if (lose < 0) return {};

    F[A[i]]--;

    For(j, i + 1, N) {
      ANS[i][j] = '1';
      ANS[j][i] = '0';
    }

    int p = N;
    Frr(x, N) {
      p -= F[x];
      if (x == 0) continue;
      int k = min(lose, F[x]);
      For(j, p, p + k) {
        ANS[i][j] = '0';
        ANS[j][i] = '1';
      }
      lose -= k;
    }
    if (lose != 0) return {};

    For(j, i + 1, N) {
      if (ANS[i][j] == '0') {
        F[A[j]]--;
        A[j]--;
        F[A[j]]++;
      }
    }
  }
  return ANS;
}
} // namespace mitsuha
#endif // AJAY_TOURNAMENT
