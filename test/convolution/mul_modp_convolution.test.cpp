#define PROBLEM "https://judge.yosupo.jp/problem/mul_modp_convolution"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/mod/multiplicative_convolution_mod_p.hpp"

using mint = modint998;

void solve() {
    INT(P);
    VT(mint, A, P);
    VT(mint, B, P);
    print(multiplicative_convolution_mod_p<mint>(P, A, B));
}

signed main() {
    solve();
    return 0;
}
