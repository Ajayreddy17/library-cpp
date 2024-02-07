#ifndef AJAY_STATIC_RANGE_PRODUCT
#define AJAY_STATIC_RANGE_PRODUCT

#include "library/datastructure/sparse_table/sparse_table.hpp"
#include "library/datastructure/sparse_table/disjoint_sparse_table.hpp"

namespace mitsuha{
/*
Reference: https://judge.yosupo.jp/submission/106668
Divide into blocks of length 2^LOG. It has prefix and suffix in the block.
It has a sequence of block products in ST(DST). The product across blocks is O(1).
For short ones, we give up on O(1) and call it naive.
Precalculation: O(Nlog(N)/2^LOG)
Query: O(1) / worst O(2^LOG)
 */
template <typename Monoid, typename SPARSE_TABLE, int LOG = 4>
struct Static_Range_Product {
    using MX = Monoid;
    using X = typename MX::value_type;
    int N, b_num;
    vector<X> A, pre, suf; // inclusive
    SPARSE_TABLE ST;

    Static_Range_Product() {}
    template <typename F>
    Static_Range_Product(int n, F f) {
        build(n, f);
    }
    Static_Range_Product(const vector<X>& v) { build(v); }

    void build(const vector<X>& v) {
        build(len(v), [&](int i) -> X { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        N = m;
        b_num = N >> LOG;
        A.resize(N);
        For(i, N) A[i] = f(i);
        pre = A, suf = A;
        constexpr int mask = (1 << LOG) - 1;
        For(i, 1, N) {
            if (i & mask) pre[i] = MX::op(pre[i - 1], A[i]);
        }
        Frr(i, 1, N) {
            if (i & mask) suf[i - 1] = MX::op(A[i - 1], suf[i]);
        }
        ST.build(b_num, [&](int i) -> X { return suf[i << LOG]; });
    }

    // O(1) or O(R-L)
    X prod(int L, int R) {
        if (L == R) return MX::unit();
        R -= 1;
        int a = L >> LOG, b = R >> LOG;
        if (a < b) {
            X x = ST.prod(a + 1, b);
            x = MX::op(suf[L], x);
            x = MX::op(x, pre[R]);
            return x;
        }
        X x = A[L];
        For(i, L + 1, R + 1) x = MX::op(x, A[i]);
        return x;
    }
};
} // namespace mitsuha
#endif // AJAY_STATIC_RANGE_PRODUCT