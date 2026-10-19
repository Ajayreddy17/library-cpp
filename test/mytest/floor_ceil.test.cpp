#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

template <typename T>
void test() {
    vector<T> X;
    T mi = numeric_limits<T>::min();
    T ma = numeric_limits<T>::max();
    for (int x = -100; x <= 100; ++x) X.emplace_back(T(x));
    for (int x = 0; x <= 100; ++x) X.emplace_back(mi + T(x));
    for (int x = 0; x <= 100; ++x) X.emplace_back(ma - T(x));
    for (auto& x: X) {
        for (auto& y: X) {
            if (y <= 0) continue;
            T k = floor(x, y);
            assert(0 <= x - k * y && x - k * y < y);
            k = ceil(x, y);
            assert(0 <= k * y - x && k * y - x < y);
        }
    }
}

void solve() {
    int a, b;
    cin >> a >> b;
    cout << a + b << "\n";
}

signed main() {
    test<int>();
    test<uint32_t>();
    test<long long>();
    test<uint64_t>();
    solve();
    return 0;
}