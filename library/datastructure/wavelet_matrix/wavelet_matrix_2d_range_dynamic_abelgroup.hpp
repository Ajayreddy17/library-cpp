#ifndef AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_ABELGROUP
#define AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_ABELGROUP

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"
#include "library/datastructure/bit_vector.hpp"

namespace mitsuha{
template <typename Monoid, typename XY, bool SMALL_X, bool SMALL_Y>
struct Wavelet_Matrix_2D_Range_Dynamic_AbelGroup {
    // Arrange the point cloud in Y ascending order.
    // Convert X to an integer and distribute it like a binary trie
    using MX = Monoid;
    using X = typename MX::value_type;
    static_assert(MX::commute);

    template <bool SMALL>
    struct TO_IDX {
        vector<XY> key;
        XY mi, ma;
        vector<int> dat;

        void build(vector<XY>& X) {
            if constexpr (SMALL) {
                mi = (X.empty() ? 0 : MIN(X));
                ma = (X.empty() ? 0 : MAX(X));
                dat.assign(ma - mi + 2, 0);
                for (auto& x: X) { dat[x - mi + 1]++; }
                For(i, len(dat) - 1) dat[i + 1] += dat[i];
            } else {
                key = X;
                sort(key.begin(), key.end());
            }
        }
        int operator()(XY x) {
            if constexpr (SMALL) {
                return dat[clamp<XY>(x - mi, 0, ma - mi + 1)];
            } else {
                return lower_bound(key.begin(), key.end(), x) - key.begin();
            }
        }
    };

    TO_IDX<SMALL_X> XtoI;
    TO_IDX<SMALL_Y> YtoI;

    int N, lg;
    vector<int> mid;
    vector<Bit_Vector> bv;
    vector<int> new_idx;
    vector<int> A;
    // fenwick tree on each tier
    vector<FenwickTree<Monoid>> dat;

    template <typename F>
    Wavelet_Matrix_2D_Range_Dynamic_AbelGroup(int N, F f) {
        build(N, f);
    }

    template <typename F>
    void build(int N_, F f) {
        N = N_;
        if (N == 0) {
            lg = 0;
            return;
        }
        vector<XY> tmp(N), Y(N);
        vector<X> S(N);
        For(i, N) tie(tmp[i], Y[i], S[i]) = f(i);
        auto I = sorted_indices(len(Y), [&](int i){ return Y[i]; });
        tmp = rearrange(tmp, I), Y = rearrange(Y, I), S = rearrange(S, I);
        XtoI.build(tmp), YtoI.build(Y);
        new_idx.resize(N);
        For(i, N) new_idx[I[i]] = i;

        lg = __lg(XtoI(*max_element(tmp.begin(), tmp.end()) + 1)) + 1;
        mid.resize(lg), bv.assign(lg, Bit_Vector(N));
        dat.resize(lg);
        A.resize(N);
        For(i, N) A[i] = XtoI(tmp[i]);

        vector<int> A0(N), A1(N);
        vector<X> S0(N), S1(N);
        Frr(d, lg) {
            int p0 = 0, p1 = 0;
            For(i, N) {
                bool f = (A[i] >> d & 1);
                if (!f) { S0[p0] = S[i], A0[p0] = A[i], p0++; }
                if (f) { S1[p1] = S[i], A1[p1] = A[i], bv[d].set(i), p1++; }
            }
            mid[d] = p0;
            bv[d].build();
            swap(A, A0), swap(S, S0);
            For(i, p1) A[p0 + i] = A1[i], S[p0 + i] = S1[i];
            dat[d].build(N, [&](int i) -> X { return S[i]; });
        }
        For(i, N) A[i] = XtoI(tmp[i]);
    }

    int count(XY x1, XY x2, XY y1, XY y2) {
        x1 = XtoI(x1), x2 = XtoI(x2);
        y1 = YtoI(y1), y2 = YtoI(y2);
        return count_inner(y1, y2, x2) - count_inner(y1, y2, x1);
    }

    X prod(XY x1, XY x2, XY y1, XY y2) { return sum(x1, x2, y1, y2); }
    X sum(XY x1, XY x2, XY y1, XY y2) {
        assert(x1 <= x2 && y1 <= y2);
        x1 = XtoI(x1), x2 = XtoI(x2);
        y1 = YtoI(y1), y2 = YtoI(y2);
        X add = sum_inner(y1, y2, x2);
        X sub = sum_inner(y1, y2, x1);
        return MX::op(add, MX::inverse(sub));
    }

    X prefix_prod(XY x, XY y) { return prefix_sum(x, y); }
    X prefix_sum(XY x, XY y) { return sum_inner(0, YtoI(y), XtoI(x)); }

    // index of the first point group given
    void add(int i, X x) {
        assert(0 <= i && i < N);
        i = new_idx[i];
        int a = A[i];
        Frr(d, lg) {
            if (a >> d & 1) {
                i = mid[d] + bv[d].rank(i, 1);
            } else {
                i = bv[d].rank(i, 0);
            }
            dat[d].add(i, x);
        }
    }

private:
    template <typename T>
    vector<T> rearrange(const vector<T> &A, const vector<int> &I) {
        vector<T> B(len(I));
        For(i, len(I)) B[i] = A[I[i]];
        return B;
    }

    int count_inner(int L, int R, int x) {
        int cnt = 0;
        Frr(d, lg) {
            int l0 = bv[d].rank(L, 0), r0 = bv[d].rank(R, 0);
            if (x >> d & 1) {
                cnt += r0 - l0, L += mid[d] - l0, R += mid[d] - r0;
            } else {
                L = l0, R = r0;
            }
        }
        return cnt;
    }

    X sum_inner(int L, int R, int x) {
        if (x == 0) return MX::unit();
        X sm = MX::unit();
        Frr(d, lg) {
            int l0 = bv[d].rank(L, 0), r0 = bv[d].rank(R, 0);
            if (x >> d & 1) {
                sm = MX::op(sm, dat[d].sum(l0, r0));
                L += mid[d] - l0, R += mid[d] - r0;
            } else {
                L = l0, R = r0;
            }
        }
        return sm;
    }
};
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_ABELGROUP