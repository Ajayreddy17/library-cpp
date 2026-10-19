#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>
#include <library/monoid/monoid_add_chmin.hpp>

void test() {
    int N = RNG(1, 100);
    vector<int> X(N);
    For(i, N) X[i] = RNG(-100, 100);
    vector<int> Y = X;
    int Q = RNG(0, 10);
    using Mono = Monoid_Add_Chmin<int>;
    using F = typename Mono::value_type;
    F f = Mono::unit();

    For(Q) {
        int t = RNG(0, 3);
        int a = RNG(-100, 100);
        if (t == 0) {
            For(i, N) Y[i] += a;
            f = Mono::op(f, Mono::add(a));
        }
        if (t == 1) {
            For(i, N) chmin(Y[i], a);
            f = Mono::op(f, Mono::chmin(a));
        }
    }
    For(i, N) assert(Y[i] == Mono::eval(f, X[i]));
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    For(100) test();
    solve();

    return 0;
}