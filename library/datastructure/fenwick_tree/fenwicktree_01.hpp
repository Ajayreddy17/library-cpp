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
    template<typename F>
    FenwickTree_01(int n, F f) {
        build(n, f);
    }

    void build(int m) {
        N = m;
        n = (N + 64) / 64;
        dat.assign(n, 0ULL);
        bit.build(n);
    }
    template<typename F>
    void build(int m, F f) {
        N = m;
        n = ceil<int>(N + 1, 64);
        dat.assign(n, 0ULL);
        For(i, N){ dat[i / 64] |= (unsigned long long)(f(i)) << (i % 64); }
        bit.build(n, [&](int i) -> int { return __builtin_popcountll(dat[i]); });
    }

    int sum_all() { return bit.sum_all(); }
    // prefix_sum
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
    void insert(int k){ add(k); }
    void erase(int k){ remove(k); }

    bool operator[](int i) const { return dat[i / 64] & (1ULL << (i % 64)); }
    bool contains(int i) const{ return (*this)[i]; }

    int kth(int k, int L = 0) {
        if (k >= sum_all()) return N;
        k += __builtin_popcountll(dat[L / 64] & ((1ULL << (L % 64)) - 1));
        L /= 64;
        int mid = 0;
        auto check = [&](auto e) -> bool {
            if (e <= k) chmax(mid, e);
            return e <= k;
        };
        int idx = bit.max_right(check, L);
        if (idx == n) return N;
        k -= mid;
        unsigned long long x = dat[idx];
        int p = __builtin_popcountll(x);
        if (p <= k) return N;
        k = binary_search([&](int n) -> bool { return (p - (x >> n)) <= k; }, 0, 64, 0);
        return 64 * idx + k;
    }

    int next(int k) {
        int idx = k / 64;
        k %= 64;
        unsigned long long x = dat[idx] & ~((1ULL << k) - 1);
        if (x) return 64 * idx + (x == 0 ? -1 : __builtin_ctzll(x));
        idx = bit.kth(0, idx + 1);
        if (idx == n || !dat[idx]) return N;
        return 64 * idx + (dat[idx] == 0 ? -1 : __builtin_ctzll(dat[idx]));
    }

    int prev(int k) {
        if (k == N) --k;
        int idx = k / 64;
        k %= 64;
        unsigned long long x = dat[idx];
        if (k < 63) x &= (1ULL << (k + 1)) - 1;
        if (x) return 64 * idx + (x == 0 ? -1 : 63 - __builtin_clzll(x));
        idx = bit.min_left([&](auto e) -> bool { return e <= 0; }, idx) - 1;
        if (idx == -1) return -1;
        return 64 * idx + (dat[idx] == 0 ? -1 : 63 - __builtin_clzll(dat[idx]));
    }
private:
    template <typename F>
    long long binary_search(F check, long long ok, long long ng, bool check_ok = true) {
        if (check_ok) assert(check(ok));
        while (abs(ok - ng) > 1) {
            auto x = (ng + ok) / 2;
            (check(x) ? ok : ng) = x;
        }
        return ok;
    }
};
} // namespace mitsuha
#endif // AJAY_FENWICKTREE_01