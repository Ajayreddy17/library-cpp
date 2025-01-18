#ifndef AJAY_WAVELET_MATRIX_2D
#define AJAY_WAVELET_MATRIX_2D

#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"

namespace mitsuha{
template <typename SEGTREE, typename XY, bool SMALL_X, bool SMALL_Y>
struct Wavelet_Matrix_2D {
    
    Wavelet_Matrix<XY, SMALL_Y, SEGTREE> WM;
    using Mono = typename SEGTREE::MX;
    using T = typename Mono::value_type;
    static_assert(Mono::commute);

    Index_Compression<XY, false, SMALL_X> IDX_X;

    int n;
    vector<int> new_idx;
    Wavelet_Matrix_2D() {}
    template <typename F>
    Wavelet_Matrix_2D(int n, F f) {
        build(n, f);
    }

    template <typename F>
    void build(int m, F f) {
        n = m;
        vector<XY> X(n), Y(n);
        vector<T> S(n);
        For(i, n) tie(X[i], Y[i], S[i]) = f(i);
        new_idx = IDX_X.build(X);
        vector<int> I(n);
        For(i, n) I[new_idx[i]] = i;
        Y = rearrange(Y, I);
        S = rearrange(S, I);
        WM.build(Y, S);
    }

    int prefix_count(XY x1, XY x2, XY y) {
        return WM.prefix_count(IDX_X(x1), IDX_X(x2), y);
    }

    int count(XY x1, XY x2, XY y1, XY y2) {
        return WM.count(IDX_X(x1), IDX_X(x2), y1, y2);
    }

    // [L,R) x [-inf, y)
    pair<int, T> prefix_count_and_prod(XY x1, XY x2, XY y) {
        return WM.prefix_count_and_prod(IDX_X(x1), IDX_X(x2), y);
    }

    // [L, R) x [y1, y2)
    pair<int, T> count_and_prod(XY x1, XY x2, XY y1, XY y2) {
        return WM.count_and_prod(IDX_X(x1), IDX_X(x2), y1, y2);
    }

    // [L, R) x [-inf, inf)
    T prod_all(XY x1, XY x2) { return WM.prod_all(IDX_X(x1), IDX_X(x2)); }
    // [L, R) x [-inf, y)
    T prefix_prod(XY x1, XY x2, XY y) {
        return WM.prefix_prod(IDX_X(x1), IDX_X(x2), y);
    }
    // [L, R) x [y1, y2)
    T prod(XY x1, XY x2, XY y1, XY y2) {
        return WM.prod(IDX_X(x1), IDX_X(x2), y1, y2);
    }

    // [L, R) x [-inf, y) largest (cnt,prod) for which check(cnt, prod) is true
    template <typename F>
    pair<int, T> max_right(XY x1, XY x2, F check) {
        return WM.max_right(IDX_X(x1), IDX_X(x2), check);
    }

    // i is the first index passed
    void set(int i, T t) { WM.set(new_idx[i], t); }
    // i is the first index passed
    void multiply(int i, T t) { WM.multiply(new_idx[i], t); }
};
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX_2D