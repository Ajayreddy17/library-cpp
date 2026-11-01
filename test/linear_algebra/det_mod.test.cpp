#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"
#include <library/template.hpp>
#include <library/linear_algebra/det.hpp>
#include "library/util/maspy_compat.hpp"
void solve() {
    LL(N, mod);
    vector<vector<int>> A(N, vector<int>(N)); read(A);
    int ANS = det_mod(A, mod);
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
