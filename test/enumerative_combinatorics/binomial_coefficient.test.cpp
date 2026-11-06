#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient"
#include "library/template.hpp"
#include "library/mod/binomial.hpp"

void solve() {
    INT(T, mod);
    Binomial X(mod);
    For(T) {
        LL(n, k);
        print(X.C(n, k));
    }
}

signed main() {
    solve();
    return 0;
}
