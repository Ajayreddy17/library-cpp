#ifndef AJAY_MODULAR_SUBSET_SUM
#define AJAY_MODULAR_SUBSET_SUM

#include "library/random/base.hpp"
#include "library/mod/modint61.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/mod/modular_subset_sum.hpp
// Faster Deterministic Modular Subset Sum. arXiv:2012.06062.
// segment tree of rolling hashes whose leaves can be cyclically shifted
// shift(k) costs O(2^(n - lsb(k)))
struct ShiftTree {
    using M61 = modint61;
    int delta;
    int N, n;
    M61 base;
    vector<M61> dat;
    vector<M61> base_pow;

    ShiftTree(int N, long long base) : delta(0), N(N), n(msb(N)), base(base) {
        assert(N == (1 << n));
        dat.assign(2 * N, 0);

        base_pow.assign(n, 1);
        base_pow[n - 1] = base;
        Frr(i, n - 1) base_pow[i] = base_pow[i + 1] * base_pow[i + 1];
    }

    inline int skew(int k) { return (delta >> (n - k)) & 1; }

    inline int left(int k, int i) {
        int mask = (1 << (k + 1)) - 1;
        return ((2 * i + 0 - skew(k + 1)) & mask) + (1 << (k + 1));
    }

    inline int right(int k, int i) {
        int mask = (1 << (k + 1)) - 1;
        return ((2 * i + 1 - skew(k + 1)) & mask) + (1 << (k + 1));
    }

    inline int parent(int k, int i) {
        int mask = (1 << k) - 1;
        return (((i + skew(k)) & mask) + (1 << k)) / 2;
    }

    inline void update(int k, int i) {
        M61 b = base_pow[k];
        dat[i] = b * dat[left(k, i)] + dat[right(k, i)];
    }

    inline void set(int i, long long x) {
        i = (i + N - delta) % N + N;
        dat[i] = x;
        int k = n;
        while (i != 1) {
            i = parent(k, i);
            --k;
            update(k, i);
        }
    }

    void shift(int k) {
        k %= N;
        if (k < 0) k += N;
        if (k == 0) return;
        int j = lsb(k);
        delta = (delta + k) % N;
        Frr(k, n - j) { For(i, 1 << k, 2 << k) update(k, i); }
    }

    // positions in [a, b) where T and Q differ, output sensitive.
    // node i of T and node j of Q both cover [x, y)
    static void find_differences(vector<int>& res, ShiftTree& T, ShiftTree& Q, int a, int b, int k, int i, int j, int x, int y) {
        if (T.dat[i] == Q.dat[j]) return;
        if (max(a, x) >= min(b, y)) return;
        if (y == x + 1) {
            res.emplace_back(x);
            return;
        }
        int z = (x + y) / 2;
        find_differences(res, T, Q, a, b, k + 1, T.left(k, i), Q.left(k, j), x, z);
        find_differences(res, T, Q, a, b, k + 1, T.right(k, i), Q.right(k, j), z, y);
    }

    static vector<int> diff(ShiftTree& T, ShiftTree& Q, int a, int b) {
        assert(T.N == Q.N);
        vector<int> res;
        find_differences(res, T, Q, a, b, 0, 1, 1, 0, T.N);
        return res;
    }
};

/*
which x in [0, mod) are sums of a subset of vals (mod mod)
O((|vals| + mod) log mod)
- can(x) / [x]: reachable or not
- restore(x): indices of a subset summing to x
*/
template <typename INT>
struct Modular_Subset_Sum {
    int mod;
    vector<INT>& vals;
    vector<int> par;

    Modular_Subset_Sum(int mod, vector<INT>& vals) : mod(mod), vals(vals) {
        for (auto&& x : vals) assert(0 <= x && x < mod);
        par.assign(mod, -1);

        const long long base = RNG(0, (1LL << 61) - 1);

        int k = 1;
        while ((1 << k) < 2 * mod) ++k;

        int L = 1 << k;
        assert(L >= 2 * mod);

        // T1: reachable set S, T2: S + x (mod mod) after shifting
        ShiftTree T1(L, base);
        ShiftTree T2(L, base);
        T1.set(0, 1);
        T2.set(0, 1);
        T2.set(L - mod, 1);

        auto bit_rev = [&](int i) -> int {
            int x = 0;
            For(k) {
                x = 2 * x + (i & 1);
                i >>= 1;
            }
            return x;
        };

        vector<vector<int>> IDS(L);
        For(i, len(vals)) { IDS[vals[i]].emplace_back(i); }

        // values in bit-reversed order keep the total shift cost O(L log L)
        For(i, 1, L) {
            int x = bit_rev(i);
            if (len(IDS[x]) == 0) continue;
            T2.shift(x - T2.delta);
            for (auto&& idx : IDS[x]) {
                auto diff = ShiftTree::diff(T1, T2, 0, mod);
                for (auto&& d : diff) {
                    if (can(d)) continue;
                    par[d] = idx;
                    T1.set(d, 1);
                    T2.set((d + x) % L, 1);
                    T2.set((L + d + x - mod) % L, 1);
                }
            }
        }
    }

    bool can(int x) {
        if (x >= mod) return false;
        return (x == 0 || par[x] != -1);
    }

    bool operator[](int x) { return can(x); }
    vector<int> restore(int x) {
        assert(can(x));
        vector<int> res;
        while (x) {
            int i = par[x];
            res.emplace_back(i);
            x -= vals[i];
            if (x < 0) x += mod;
        }
        reverse(res.begin(), res.end());
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_MODULAR_SUBSET_SUM
