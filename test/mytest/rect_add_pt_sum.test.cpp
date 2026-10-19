#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>
#include <library/random/base.hpp>

#include <library/datastructure/offline_query/rectangle_add_point_sum.hpp>
#include <library/mod/modint.hpp>

using mint = modint998;
using QT = tuple<long long, long long, long long, long long, long long>;

pair<vector<QT>, vector<pair<int, int>>> gen(int H, int W, int Q) {
    vector<tuple<long long, long long, long long, long long, long long>> add_query;
    For(Q) {
        long long a = RNG(0, H);
        long long b = RNG(a + 1, H + 1);
        long long c = RNG(0, W);
        long long d = RNG(c + 1, W + 1);
        long long x = RNG(0, mint::get_mod());
        add_query.emplace_back(a, b, c, d, x);
    }

    vector<pair<int, int>> sum_query;
    For(Q) {
        long long x = RNG(0, H), y = RNG(0, W);
        sum_query.emplace_back(x, y);
    }
    return {add_query, sum_query};
}

vector<mint> sol_1(int H, int W, vector<QT> add_query, vector<pair<int, int>> sum_query) {
    vector<vector<mint>> A(H, vector<mint>(W));
    for (auto&& [a, b, c, d, x] : add_query) {
        For(i, a, b) For(j, c, d) { A[i][j] += mint(x); }
    }
    vector<mint> ANS;
    for (auto&& [x, y] : sum_query) ANS.emplace_back(A[x][y]);
    return ANS;
}

vector<mint> sol_2(int H, int W, vector<QT> add_query, vector<pair<int, int>> sum_query) {
    vector<mint> ANS;
    for (auto&& [x, y] : sum_query) {
        mint ans = 0;
        for (auto&& [a, b, c, d, v] : add_query) {
            if (a <= x && x < b && c <= y && y < d) ans += mint(v);
        }
        ANS.emplace_back(ans);
    }
    return ANS;
}

void test() {
    For(H, 1, 10) For(W, 1, 10) For(Q, 10) {
        auto [add_query, sum_query] = gen(H, W, Q);
        Rectangle_Add_Point_Sum<Monoid_Add<mint>, int, 0> X;
        for (auto&& [a, b, c, d, v] : add_query) X.add_query(a, b, c, d, v);
        for (auto&& [a, b] : sum_query) X.sum_query(a, b);
        assert(X.calc() == sol_1(H, W, add_query, sum_query));
    }
    For(H, 1, 10) For(W, 1, 10) For(Q, 10) {
        auto [add_query, sum_query] = gen(H, W, Q);
        Rectangle_Add_Point_Sum<Monoid_Add<mint>, int, 1> X;
        for (auto&& [a, b, c, d, v] : add_query) X.add_query(a, b, c, d, v);
        for (auto&& [a, b] : sum_query) X.sum_query(a, b);
        assert(X.calc() == sol_1(H, W, add_query, sum_query));
    }
    For(10) {
        int H = RNG(1, 1'000'000'000);
        int W = RNG(1, 1'000'000'000);
        int Q = 100;
        auto [add_query, sum_query] = gen(H, W, Q);
        Rectangle_Add_Point_Sum<Monoid_Add<mint>, int, 0> X;
        for (auto&& [a, b, c, d, v] : add_query) X.add_query(a, b, c, d, v);
        for (auto&& [a, b] : sum_query) X.sum_query(a, b);
        assert(X.calc() == sol_2(H, W, add_query, sum_query));
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
