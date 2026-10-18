#define PROBLEM "https://judge.yosupo.jp/problem/chromatic_number"
#include <library/template.hpp>

#include <library/graph/base.hpp>
#include <library/graph/chromatic.hpp>

void solve() {
    LL(N, M);
    Graph<int> G(N);
    G.read_graph(M, 0, 0);
    print(chromatic_number(G));
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
