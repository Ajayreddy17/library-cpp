#define PROBLEM "https://judge.yosupo.jp/problem/gcd_convolution"
#include <library/template.hpp>
#include <library/mod/modint.hpp>
#include <library/number/gcd_convolution.hpp>

using mint = modint998;
void solve() {
    LL(N);
    VT(mint, A, N);
    VT(mint, B, N);
    A.insert(A.begin(), 0);
    B.insert(B.begin(), 0);
    A = gcd_convolution(A, B);
    A.erase(A.begin());
    print(A);
}

signed main() {
    solve();

    return 0;
}
