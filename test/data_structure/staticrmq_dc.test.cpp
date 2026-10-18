#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include <library/template.hpp>
#include <library/monoid/monoid_min.hpp>
#include <library/datastructure/offline_query/static_monoid_products.hpp>

void solve() {
    INT(N, Q);
    VT(int, A, N);
    vector<pair<int, int>> LR(Q);
    For(q, Q) read(LR[q]);
    vector<int> ANS(Q);
    static_monoid_products<Monoid_Min<int>>(A, LR, [&](int q, int lx, int rx) -> void { ANS[q] = min(lx, rx); });
    for (auto& x: ANS) print(x);
}

signed main() {
    solve();

    return 0;
}
