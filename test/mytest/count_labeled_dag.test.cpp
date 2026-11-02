#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include "library/template.hpp"
#include "library/mod/modint.hpp"
#include "library/graph/count/count_labeled_dag.hpp"

using mint = modint998;

void test() {
    vector<mint> F = count_labeled_dag<mint>(10);
    vector<long long> ANS = {
            1,
            1,
            3,
            25,
            543,
            29281,
            3781503,
            1138779265,
            783702329343,
            1213442454842881,
            4175098976430598143,
    };
    For(n, 11) assert(F[n] == mint(ANS[n]));
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test();
    solve();
    return 0;
}
