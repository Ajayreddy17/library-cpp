#define PROBLEM "https://judge.yosupo.jp/problem/pow_of_matrix"
#include <library/template.hpp>
#include <library/linear_algebra/matrix_pow.hpp>
#include <library/mod/modint.hpp>
using mint = modint998;

void solve() {
    INT(N);
    unsigned long long K;
    read(K);
    vector<vector<mint>> A(N, vector<mint>(N)); read(A);
    A = matrix_pow(A, K);
    For(i, N) print(A[i]);
}

signed main() {
    solve();
    return 0;
}
