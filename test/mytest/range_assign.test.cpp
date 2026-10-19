#define PROBLEM "https://judge.yosupo.jp/problem/aplusb"
#include <library/template.hpp>

#include <library/datastructure/segment_tree/range_assignment_segtree.hpp>
#include <library/datastructure/segment_tree/lazy_segtree.hpp>
#include <library/monoid/monoid_add.hpp>
#include <library/acted_monoid/am_sum_assign.hpp>
#include <library/random/base.hpp>

struct PROB {
    int N, Q;
    vector<long long> INIT;
    vector<tuple<int, int, int>> QUERY;
};

PROB gen(int N, int Q) {
    PROB p;
    p.N = N, p.Q = Q;
    For(N) { p.INIT.emplace_back(RNG(0, 1 << 30)); }
    For(Q) {
        int t = RNG(0, 2);
        int l = RNG(0, N), r = RNG(0, N);
        int x = RNG(0, 1 << 30);
        if (l > r) swap(l, r);
        ++r;
        if (t == 0) p.QUERY.emplace_back(l, r, x);
        if (t == 1) p.QUERY.emplace_back(l, r, -1);
    }
    return p;
}

vi sol_1(PROB p) {
    vi ANS;
    Lazy_SegTree<ActedMonoid_Sum_Assign<long long, -1>> seg(p.INIT);
    for (auto& [l, r, x]: p.QUERY) {
        if (x == -1) {
            ANS.emplace_back(seg.prod(l, r));
        } else {
            seg.apply(l, r, x);
        }
    }
    return ANS;
}

vi sol_2(PROB p) {
    vi ANS;
    Range_Assignment_SegTree<Monoid_Add<long long>> seg(p.INIT);
    for (auto& [l, r, x]: p.QUERY) {
        if (x == -1) {
            ANS.emplace_back(seg.prod(l, r));
        } else {
            seg.assign(l, r, x);
        }
    }
    return ANS;
}

void test() {
    int N = 1 << 22, Q = 1 << 22;
    PROB p = gen(N, Q);
    double a = clock();
    vi A = sol_1(p);
    double b = clock();
    vi B = sol_2(p);
    double c = clock();
    a = (b - a) / CLOCKS_PER_SEC;
    b = (c - b) / CLOCKS_PER_SEC;
    assert(A == B);
    // cout << a << "\n"; 1.563 sec
    // cout << b << "\n"; 1.376 sec
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