#define PROBLEM "https://judge.yosupo.jp/problem/mul_mod2n_convolution"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/mod/multiplicative_convolution_mod_2n.hpp"

using mint = modint998;

void solve() {
    LL(N);
    VT(mint, A, 1 << N);
    VT(mint, B, 1 << N);
    print(multiplicative_convolution_mod_2n(A, B));
}

signed main() {
    solve();
    return 0;
}
