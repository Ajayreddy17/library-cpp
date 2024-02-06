#ifndef AJAY_STATIC_RMQ
#define AJAY_STATIC_RMQ

#include "library/datastructure/sparse_table/sparse_table.hpp"

namespace mitsuha{
// Construction O(N), query O(1)
template <typename Monoid>
struct Static_RMQ {
    using MX = Monoid;
    using X = typename MX::value_type;
    static constexpr int LOG = 4;
    int N, b_num;
    vector<X> A, pre, suf; // inclusive
    Sparse_Table<Monoid> ST;

    using u16 = unsigned short;
    vector<u16> dat;

    Static_RMQ() {}
    template <typename F>
    Static_RMQ(int n, F f) {
        build(n, f);
    }
    Static_RMQ(const vector<X>& v) { build(v); }

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
        For(i, 1, N) {
            if (i & 15) pre[i] = MX::op(pre[i - 1], A[i]);
        }
        Frr(i, 1, N) {
            if (i & 15) suf[i - 1] = MX::op(A[i - 1], suf[i]);
        }
        ST.build(b_num, [&](int i) -> X { return suf[i << LOG]; });
        // Precomputation to accommodate queries of length 16 or less
        // Set j-th bit if i+j is the minimum value in [i,i+j] in [i,i+16)
        dat.resize(N);
        unsigned int stack = 0;
        Frr(i, N) {
            stack = (stack << 1) & 65535;
            while (stack) {
                int k = (stack == 0 ? -1 : __builtin_ctz(stack));
                if (MX::op(A[i], A[i + k]) != A[i]) break;
                stack &= ~(1U << k);
            }
            stack |= 1U;
            dat[i] = stack;
        }
    }

    X prod(int L, int R) {
        assert(0 <= L && L <= R && R <= N);
        if (L == R) return MX::unit();
        if (R - L <= 16) {
            unsigned int d = dat[L] & ((1U << (R - L)) - 1);
            return A[L + (d == 0 ? -1 : 31 - __builtin_clz(d))];
        }
        --R;
        int a = L >> LOG, b = R >> LOG;
        X x = ST.prod(a + 1, b);
        x = MX::op(suf[L], x);
        x = MX::op(x, pre[R]);
        return x;
    }
};
} // namespace mitsuha
#endif // AJAY_STATIC_RMQ