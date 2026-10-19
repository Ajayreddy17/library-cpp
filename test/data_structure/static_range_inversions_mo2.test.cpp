#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree.hpp>
#include <library/datastructure/offline_query/rollback_mo.hpp>

void solve() {
    LL(N, Q);
    VT(long long, A, N);
    vector<long long> key = A;
    settify(key);

    for (auto&& x: A) x = lower_bound(key.begin(), key.end(), x) - key.begin();
    long long K = len(key);
    FenwickTree<Monoid_Add<int>> bit(K);

    Rollback_Mo mo;
    vector<long long> ANS(Q);
    For(Q) {
        LL(L, R);
        mo.add(L, R);
    }

    long long inv = 0;
    long long tinv = 0;
    long long t = 0;
    vector<int> history;
    auto add_l = [&](int i) -> void {
        int x = A[i];
        inv += bit.sum(x);
        bit.add(x, +1);
        history.emplace_back(x);
    };
    auto add_r = [&](int i) -> void {
        int x = A[i];
        inv += bit.sum_all() - bit.sum(x + 1);
        bit.add(x, +1);
        history.emplace_back(x);
    };
    auto rb = [&](int t) -> void {
        while (len(history) > t) { int x = history.back(); history.pop_back(); bit.add(x, -1); }
    };
    auto reset = [&]() -> void {
        rb(0);
        inv = 0;
    };
    auto save = [&]() -> void {
        t = len(history);
        tinv = inv;
    };
    auto rollback = [&]() -> void {
        rb(t);
        inv = tinv;
    };
    auto calc = [&](int i) -> void { ANS[i] = inv; };
    mo.calc(add_l, add_r, reset, save, rollback, calc);
    for (auto&& x: ANS) print(x);
}

signed main() {
    cin.tie(nullptr);
    ios::sync_with_stdio(false);
    cout << setprecision(15);

    solve();

    return 0;
}
