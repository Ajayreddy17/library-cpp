#define PROBLEM "https://judge.yosupo.jp/problem/kth_root_mod"
#include "library/template.hpp"
#include "library/mod/mod_kth_root.hpp"

void solve() {
    LL(k, y, p);
    print(mod_kth_root(k, y % p, p));
}

signed main() {
    LL(T);
    For(T) solve();
    return 0;
}
