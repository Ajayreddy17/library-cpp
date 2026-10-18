#define PROBLEM "https://judge.yosupo.jp/problem/matrix_det_arbitrary_mod"
#include <library/template.hpp>

#include <library/linear_algebra/det.hpp>

void solve() {
    LL(N, mod);
    VV(int, A, N, N);
    int ANS = det_mod(A, mod);
    print(ANS);
}

signed main() {
    solve();

    return 0;
}
