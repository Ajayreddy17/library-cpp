#define PROBLEM "https://judge.yosupo.jp/problem/primitive_root"
#include <library/template.hpp>
#include <library/mod/primitive_root.hpp>

void solve() {
    LL(p);
    long long ANS = primitive_root_64(p);
    print(ANS);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    LL(T);
    For(T) solve();

    return 0;
}
