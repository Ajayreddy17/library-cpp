#define PROBLEM "https://yukicoder.me/problems/no/1167"
#include "library/template.hpp"
#include "library/datastructure/offline_query/range_xor_basis_query.hpp"
#include "library/mod/modint.hpp"

using mint = modint107;

void solve() {
  LL(N, Q);
  vector<uint32_t> A(N); read(A);
  vector<vector<pair<long long, long long>>> query(Q);
  For(q, Q) {
    LL(M);
    vector<long long> B(M); read(B);
    B.insert(B.begin(), 0);
    B.emplace_back(N);
    For(k, len(B) - 1) query[q].emplace_back(B[k], B[k + 1]);
  }

  Range_Xor_Basis_Query<uint32_t, 32> X(A);
  For(q, Q) {
    for (auto&& [l, r]: query[q]) { X.query(l, r); }
  }

  vector<int> dims(X.Q);
  auto CALC = [&](int q, vector<uint32_t> basis) -> void { dims[q] = len(basis); };

  X.calc(CALC);
  int p = 0;
  For(q, Q) {
    int e = 0;
    for (auto&& [l, r]: query[q]) {
      int d = dims[p++];
      e += r - l - d;
    }
    print(mint(2).pow(e));
  }
}

signed main() {
  solve();
  return 0;
}
