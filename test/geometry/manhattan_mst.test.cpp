#define PROBLEM "https://judge.yosupo.jp/problem/manhattanmst"
#include <library/template.hpp>

#include <library/geo/manhattan_mst.hpp>

void solve() {
    LL(N);
    using P = pair<int, int>;
    VT(P, XY, N);
    auto G = manhattan_mst<int>(XY);
    long long ANS = 0;
    for (auto&& e: G.edges) { ANS += e.cost; }
    print(ANS);
    for (auto&& e: G.edges) { print(e.frm, e.to); }
}

signed main() {
    cout << fixed << setprecision(15);

    long long T = 1;
    // LL(T);
    For(T) solve();

    return 0;
}
