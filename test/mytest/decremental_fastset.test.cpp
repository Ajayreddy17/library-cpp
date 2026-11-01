#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/random/shuffle.hpp"
#include "library/util/timer.hpp"
#include "library/datastructure/fastset.hpp"
#include "library/datastructure/decremental_fastset.hpp"

// ackermann; uses more memory
struct Decremental_FastSet_UF_ONLY {
  int n;
  UnionFind uf;
  vector<int> L, R;
  Decremental_FastSet_UF_ONLY(int n) : n(n), uf(n + 2), L(n + 2), R(n + 2) {
    For(i, n + 2) L[i] = i, R[i] = i;
  }
  void erase(int i) {
    assert(0 <= i && i < n);
    ++i;
    int l = L[uf[i - 1]], r = R[uf[i]];
    uf.merge(i, i - 1);
    L[uf[i]] = l, R[uf[i]] = r;
  }
  int prev(int i) {
    assert(-1 <= i);
    chmin(i, n - 1);
    return L[uf[i + 1]] - 1;
  }
  int next(int i) {
    assert(i <= n);
    chmax(i, 0);
    return R[uf[i]];
  }
};

vector<pair<int, int>> sol1(vector<int> A, vector<int> B) {
  int N = len(A);
  FastSet FS(N, [&](int i) -> int { return 1; });
  vector<pair<int, int>> ANS(N);
  For(i, N) {
    FS.erase(A[i]);
    ANS[i] = {FS.prev(B[i]), FS.next(B[i])};
  }
  return ANS;
}

vector<pair<int, int>> sol2(vector<int> A, vector<int> B) {
  int N = len(A);
  Decremental_FastSet FS(N);
  vector<pair<int, int>> ANS(N);
  For(i, N) {
    FS.erase(A[i]);
    ANS[i] = {FS.prev(B[i]), FS.next(B[i])};
  }
  return ANS;
}

vector<pair<int, int>> sol3(vector<int> A, vector<int> B) {
  int N = len(A);
  Decremental_FastSet_UF_ONLY FS(N);
  vector<pair<int, int>> ANS(N);
  For(i, N) {
    FS.erase(A[i]);
    ANS[i] = {FS.prev(B[i]), FS.next(B[i])};
  }
  return ANS;
}

void test() {
  vector<double> X, Y, Z;
  For(100) {
    int N = 1 << 20;
    vector<int> A(N);
    For(i, N) A[i] = i;
    shuffle(A);
    vector<int> B(N);
    For(i, N) B[i] = RNG(0, N);
    vector<pair<int, int>> ANS1, ANS2, ANS3;
    {
      Timer timer;
      timer.start();
      ANS1 = sol1(A, B);
      X.emplace_back(timer());
    }
    {
      Timer timer;
      timer.start();
      ANS2 = sol2(A, B);
      Y.emplace_back(timer());
    }

    {
      Timer timer;
      timer.start();
      ANS3 = sol3(A, B);
      Z.emplace_back(timer());
    }
    // print(X.back(), Y.back(), Z.back());

    assert(ANS1 == ANS2);
    assert(ANS1 == ANS3);
  }
  // print(accumulate(X.begin(), X.end(), double(0)));
  // print(accumulate(Y.begin(), Y.end(), double(0)));
  // print(accumulate(Z.begin(), Z.end(), double(0)));
}

void solve() {
  int a, b;
  cin >> a >> b;
  cout << a + b << "\n";
}

signed main() {
  test();
  solve();
}
