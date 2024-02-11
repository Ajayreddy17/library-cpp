#ifndef AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_MONOID
#define AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_MONOID

#include "library/datastructure/segment_tree/segtree.hpp"
#include "library/datastructure/bit_vector.hpp"

namespace mitsuha{
template <typename Monoid, typename XY, bool SMALL_X, bool SMALL_Y>
struct Wavelet_Matrix_2D_Range_Dynamic_Monoid {
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
    vector<SegTree<Monoid>> dat;

    template <typename F>
    Wavelet_Matrix_2D_Range_Dynamic_Monoid(int N, F f) {
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

        lg = __lg(XtoI(MAX(tmp) + 1)) + 1;
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
        return prefix_count(y1, y2, x2) - prefix_count(y1, y2, x1);
    }

    X prod(XY x1, XY x2, XY y1, XY y2) {
        assert(x1 <= x2 && y1 <= y2);
        x1 = XtoI(x1), x2 = XtoI(x2);
        y1 = YtoI(y1), y2 = YtoI(y2);
        X res = MX::unit();
        prod_dfs(y1, y2, x1, x2, lg - 1, res);
        return res;
    }

    // index of the first point group given
    void set(int i, X x) {
        assert(0 <= i && i < N);
        i = new_idx[i];
        int a = A[i];
        Frr(d, lg) {
            if (a >> d & 1) {
                i = mid[d] + bv[d].rank(i, 1);
            } else {
                i = bv[d].rank(i, 0);
            }
            dat[d].set(i, x);
        }
    }
    void multiply(int i, X x) {
        assert(0 <= i && i < N);
        i = new_idx[i];
        int a = A[i];
        Frr(d, lg) {
            if (a >> d & 1) {
                i = mid[d] + bv[d].rank(i, 1);
            } else {
                i = bv[d].rank(i, 0);
            }
            dat[d].multiply(i, x);
        }
    }

private:
    template <typename T>
    vector<T> rearrange(const vector<T> &A, const vector<int> &I) {
        vector<T> B(len(I));
        For(i, len(I)) B[i] = A[I[i]];
        return B;
    }
    
    int prefix_count(int L, int R, int x) {
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

    void prod_dfs(int L, int R, int x1, int x2, int d, X& res) {
        chmax(x1, 0), chmin(x2, 1 << (d + 1));
        if (x1 >= x2) { return; }
        assert(0 <= x1 && x1 < x2 && x2 <= (1 << (d + 1)));
        if (x1 == 0 && x2 == (1 << (d + 1))) {
            res = MX::op(res, dat[d + 1].prod(L, R));
            return;
        }
        int l0 = bv[d].rank(L, 0), r0 = bv[d].rank(R, 0);
        prod_dfs(l0, r0, x1, x2, d - 1, res);
        prod_dfs(L + mid[d] - l0, R + mid[d] - r0, x1 - (1 << d), x2 - (1 << d), d - 1, res);
    }
};
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX_2D_RANGE_DYNAMIC_MONOID