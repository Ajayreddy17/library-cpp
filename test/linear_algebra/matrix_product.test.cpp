#define PROBLEM "https://judge.yosupo.jp/problem/matrix_product"
#include <library/template.hpp>
#include <library/mod/modint.hpp>
#include <library/linear_algebra/matrix_mul.hpp>
using mint = modint998;

void solve() {
    LL(N, M, K);
    vector<vector<mint>> A(N, vector<mint>(M)); read(A);
    vector<vector<mint>> B(M, vector<mint>(K)); read(B);
    auto C = matrix_mul(A, B);
    For(n, len(C)) print(C[n]);
}

signed main() {
    solve();
    return 0;
}
