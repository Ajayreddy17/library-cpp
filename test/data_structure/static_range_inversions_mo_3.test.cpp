#define PROBLEM "https://judge.yosupo.jp/problem/static_range_inversions_query"
#include <library/template.hpp>

#include <library/datastructure/fenwick_tree/fenwicktree_01.hpp>
#include <library/datastructure/offline_query/mo.hpp>

void solve() {
    LL(N, Q);
    VT(int, A, N);
    auto I = argsort(A);
    vector<int> B(N);
    For(i, N) B[I[i]] = i;
    swap(A, B);

    FenwickTree_01 bit(N);

    Mo mo;
    vector<long long> ANS(Q);
    For(Q) {
        LL(L, R);
        mo.add(L, R);
    }

    long long inv = 0;
    auto add_l = [&](int i) -> void {
        int x = A[i];
        inv += bit.sum(x);
        bit.add(x, +1);
    };
    auto rm_l = [&](int i) -> void {
        int x = A[i];
        bit.add(x, -1);
        inv -= bit.sum(x);
    };
    auto add_r = [&](int i) -> void {
        int x = A[i];
        inv += bit.sum_all() - bit.sum(x + 1);
        bit.add(x, +1);
    };
    auto rm_r = [&](int i) -> void {
        int x = A[i];
        bit.add(x, -1);
        inv -= bit.sum_all() - bit.sum(x + 1);
    };
    auto calc = [&](int i) -> void { ANS[i] = inv; };
    mo.calc(add_l, add_r, rm_l, rm_r, calc);
    for (auto&& x: ANS) print(x);
}

signed main() {
    solve();

    return 0;
}
