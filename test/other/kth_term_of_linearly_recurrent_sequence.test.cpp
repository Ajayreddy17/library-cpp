#define PROBLEM \
    "https://judge.yosupo.jp/problem/kth_term_of_linearly_recurrent_sequence"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/poly/coef_of_rational_fps.hpp>

using mint = modint998;
void solve() {
    LL(N, K);
    VT(mint, A, N);
    VT(mint, g, N);
    for (auto&& x : g) x = -x;
    g.insert(g.begin(), 1);
    auto f = convolution(A, g);
    f.resize(N);
    print(coef_of_rational_fps(f, g, K));
}

signed main() {
    solve();

    return 0;
}
