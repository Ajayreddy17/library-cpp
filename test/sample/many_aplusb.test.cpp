#define PROBLEM "https://judge.yosupo.jp/problem/many_aplusb"
#include <library/template.hpp>

void solve() {
    LL(T);
    For(T) {
        LL(a, b);
        print(a + b);
    }
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
