#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/number/digit_sum.hpp>
#include <library/random/base.hpp>

int naive(uint64_t x) {
    int res = 0;
    while (x) {
        res += x % 10;
        x /= 10;
    }
    return res;
}

void test() {
    For(x, 1000000) { assert(naive(x) == digit_sum(x)); }
    For(1000000) {
        uint64_t x = RNG_64();
        assert(naive(x) == digit_sum(x));
    }
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
