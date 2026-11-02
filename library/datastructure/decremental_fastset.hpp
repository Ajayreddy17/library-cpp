#ifndef AJAY_DECREMENTAL_FASTSET
#define AJAY_DECREMENTAL_FASTSET

#include "library/datastructure/unionfind/unionfind.hpp"
#include "library/util/bit_utils.hpp"

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/ds/decremental_fastset.hpp

// amortized linear
// without MoFR it was slower than FastSet
struct Decremental_FastSet {
    struct Decremental_Neighbor_UF {
        int n;
        UnionFind uf;
        vector<int> L, R;
        Decremental_Neighbor_UF(int n) : n(n), uf(n + 2), L(n + 2), R(n + 2) {
            For(i, n + 2) L[i] = i, R[i] = i;
        }
        void erase(int i) {
            assert(0 <= i && i < n);
            ++i;
            int l = L[uf[i - 1]], r = R[uf[i]];
            uf.merge(i, i - 1);
            L[uf[i]] = l, R[uf[i]] = r;
        }
        int prev(int i) {
            assert(-1 <= i);
            chmin(i, n - 1);
            return L[uf[i + 1]] - 1;
        }
        int next(int i) {
            assert(i <= n);
            chmax(i, 0);
            return R[uf[i]];
        }
    };
    int N, n;
    vector<uint64_t> dat;
    Decremental_Neighbor_UF X;
    Decremental_FastSet(int N) : N(N), n((N + 63) / 64), X(n) {
        dat.assign(n, uint64_t(-1));
        if (n) dat.back() = uint64_t(-1) >> (64 * n - N);
    }

    bool operator[](int i) { return (dat[i / 64] >> (i & 63) & 1); }

    void erase(int i) {
        int a = i / 64, b = i & 63;
        if (!(dat[a] >> b & 1)) return;
        dat[a] &= ~(uint64_t(1) << b);
        if (dat[a] == 0) {
            X.erase(a);
        }
    }
    int prev(int i) {
        assert(-1 <= i);
        chmin(i, N - 1);
        if (i == -1) return -1;
        int a = i / 64, b = i & 63;
        uint64_t x = dat[a] & (uint64_t(-1) >> (63 - b));
        if (x != 0) return 64 * a + msb(x);
        a = X.prev(a - 1);
        return (a == -1 ? -1 : 64 * a + msb(dat[a]));
    }
    int next(int i) {
        assert(i <= N);
        chmax(i, 0);
        if (i == N) return N;
        int a = i / 64, b = i & 63;
        uint64_t x = dat[a] >> b;
        if (x != 0) return 64 * a + b + lsb(x);
        a = X.next(a + 1);
        return (a == n ? N : 64 * a + lsb(dat[a]));
    }

    // [l, r)
    template <typename F>
    void enumerate(int l, int r, F&& f) {
        for (int x = next(l); x < r; x = next(x + 1)) f(x);
    }

    string to_string() {
        string S(N, '.');
        For(i, N) S[i] = '0' + (dat[i / 64] >> (i & 63) & 1);
        return S;
    }
};
} // namespace mitsuha
#endif // AJAY_DECREMENTAL_FASTSET
