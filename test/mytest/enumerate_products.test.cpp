#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/enumerate/product.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;

void test() {
    {
        vector<vector<int>> res;
        auto f = [&](vector<int> A) -> void { res.emplace_back(A); };
        enumerate_product(vector<int>(2, 3), f);
        assert(len(res) == 9);
        assert(res[0] == vector<int>({0, 0}));
        assert(res[1] == vector<int>({0, 1}));
        assert(res[2] == vector<int>({0, 2}));
        assert(res[3] == vector<int>({1, 0}));
        assert(res[4] == vector<int>({1, 1}));
        assert(res[5] == vector<int>({1, 2}));
        assert(res[6] == vector<int>({2, 0}));
        assert(res[7] == vector<int>({2, 1}));
        assert(res[8] == vector<int>({2, 2}));
    }
    {
        int cnt = 0;
        auto f = [&](vector<int> A) -> void { ++cnt; };
        enumerate_product(vector<int>(4, 3), f);
        assert(cnt == 81);
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
