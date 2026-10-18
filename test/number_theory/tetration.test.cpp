#define PROBLEM "https://judge.yosupo.jp/problem/tetration_mod"
#include <library/template.hpp>
#include <library/mod/tetration.hpp>

void solve() {
    LL(a, b, m);
    if (a == 0) {
        long long ANS = (b % 2 == 0 ? 1 : 0);
        return print(ANS % m);
    }
    chmin(b, 64);
    vector<long long> v(b, a);
    print(tetration(v, m));
}

signed main() {
    LL(T);
    For(T) solve();

    return 0;
}
