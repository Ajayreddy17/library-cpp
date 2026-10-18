#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb_128bit"
#include <library/template.hpp>

#include <library/bigint/base.hpp>

using bint = BigInteger;

void solve() {
    STR(A, B);
    bint a(A), b(B);
    print((a + b).to_string());
}

signed main() {
    INT(T);
    For(T) solve();
    return 0;
}
