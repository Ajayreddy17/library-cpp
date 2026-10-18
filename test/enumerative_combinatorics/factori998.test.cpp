#define PROBLEM "https://judge.yosupo.jp/problem/factorial"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/mod/factorial998.hpp>

using mint = modint998;

void solve() {
    INT(N);
    print(factorial998(N));
}

signed main() {
    INT(T);
    For(T) solve();
    return 0;
}
