#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/count/count_labeled_eulerian.hpp"

using mint = modint998;

void test() {
    vector<mint> F = count_labeled_eulerian<mint>(10);
    vector<long long> ANS = {0, 1, 0, 1, 3, 38, 720, 26614, 1858122, 250586792, 66121926720};
    For(n, 11) assert(F[n] == mint(ANS[n]));
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << '\n';
}

signed main() {
    test();
    solve();
    return 0;
}
