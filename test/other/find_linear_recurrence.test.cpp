#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"
#include <library/template.hpp>

#include <library/mod/modint.hpp>
#include <library/seq/find_linear_rec.hpp>

using mint = modint998;
void solve() {
    LL(N);
    VT(mint, A, N);
    auto g = find_linear_rec(A);
    g.erase(g.begin());
    for (auto&& x: g) x = -x;
    print(len(g));
    print(g);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
