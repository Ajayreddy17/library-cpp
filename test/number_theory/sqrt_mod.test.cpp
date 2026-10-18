#define PROBLEM "https://judge.yosupo.jp/problem/sqrt_mod"
#include <library/template.hpp>

#include <library/mod/mod_sqrt.hpp>

void solve() {
    INT(y, p);
    print(mod_sqrt(y, p));
}

signed main() {
    INT(T);
    For(T) solve();
    return 0;
}
