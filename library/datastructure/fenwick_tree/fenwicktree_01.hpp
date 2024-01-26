#ifndef AJAY_FENWICKTREE_01
#define AJAY_FENWICKTREE_01

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
struct FenwickTree_01 {
    int N, n;
    vector<unsigned long long> dat;
    FenwickTree<Monoid_Add<int>> bit;
    FenwickTree_01() {}
    FenwickTree_01(int n) { build(n); }
    template <typename F>
    FenwickTree_01(int n, F f) {
        build(n, f);
    }

    void build(int m) {
        N = m;
        n = ceil(N + 1, 64);
        dat.assign(n, 0ULL);
        bit.build(n);
    }

    template <typename F>
    void build(int m, F f) {
        N = m;
        n = ceil(N + 1, 64);
        dat.assign(n, 0ULL);
        For(i, N) { dat[i / 64] |= 1ULL << (i % 64); }
        bit.build(n, [&](int i) -> int { return __builtin_popcountll(dat[i]); });
    }

    int sum_all() { return bit.prod_all(); }
    int sum(int k) { return prefix_sum(k); }
    int prefix_sum(int k) {
        int ans = bit.sum(k / 64);
        ans += __builtin_popcountll(dat[k / 64] & ((1ULL << (k % 64)) - 1));
        return ans;
    }
    int sum(int L, int R) {
        if (L == 0) return prefix_sum(R);
        int ans = 0;
        ans -= __builtin_popcountll(dat[L / 64] & ((1ULL << (L % 64)) - 1));
        ans += __builtin_popcountll(dat[R / 64] & ((1ULL << (R % 64)) - 1));
        ans += bit.sum(L / 64, R / 64);
        return ans;
    }

    void add(int k, int x) {
        if (x == 1) add(k);
        if (x == -1) remove(k);
    }

    void add(int k) {
        dat[k / 64] |= 1ULL << (k % 64);
        bit.add(k / 64, 1);
    }
    void remove(int k) {
        dat[k / 64] &= ~(1ULL << (k % 64));
        bit.add(k / 64, -1);
    }
};
} // namespace mitsuha
#endif // AJAY_FENWICKTREE_01