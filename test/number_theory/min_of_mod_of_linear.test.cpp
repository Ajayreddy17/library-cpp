#define PROBLEM "https://judge.yosupo.jp/problem/min_of_mod_of_linear"
#include "library/template.hpp"
#include "library/mod/min_of_linear.hpp"

void solve() {
    LL(n, m, a, b);
    auto [x, fx] = min_of_linear(0, n, a, b, m);
    print(fx);
}

signed main() {
    LL(T);
    For(T) solve();
    return 0;
}
