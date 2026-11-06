#ifndef AJAY_WAVELET_MATRIX_2D_RANGE
#define AJAY_WAVELET_MATRIX_2D_RANGE

#include "library/datastructure/wavelet_matrix/wavelet_matrix.hpp"
#include "library/datastructure/index_compression.hpp"

namespace mitsuha{
// Points (x, y) with weights; queries over [x1, x2) x [y1, y2).
// Points are sorted by x, so a wavelet matrix over y answers the rectangle.
// compress_X = false: x are small integers (dense compression); compress_Y as in Wavelet_Matrix.
template <typename XY, bool compress_X, bool compress_Y, typename SEGTREE = Dummy_Data_Structure>
struct Wavelet_Matrix_2D_Range {
    Wavelet_Matrix<XY, compress_Y, SEGTREE> WM;
    using Mono = typename SEGTREE::MX;
    using T = typename Mono::value_type;
    static_assert(Mono::commute);

    Index_Compression<XY, false, !compress_X> IDX_X;

    int n;
    vector<int> new_idx;

    Wavelet_Matrix_2D_Range() {}
    // f(i) = {x, y, dat}
    template <typename F>
    Wavelet_Matrix_2D_Range(int n, F f) {
        build(n, f);
    }

    template <typename F>
    void build(int m, F f) {
        n = m;
        vector<XY> X(n), Y(n);
        vector<T> S(n);
        For(i, n) {
            auto tmp = f(i);
            X[i] = get<0>(tmp), Y[i] = get<1>(tmp), S[i] = get<2>(tmp);
        }
        new_idx = IDX_X.build(X);
        vector<int> I(n);
        For(i, n) I[new_idx[i]] = i;
        Y = rearrange(Y, I);
        S = rearrange(S, I);
        WM.build(n, [&](int i) -> pair<XY, T> { return {Y[i], S[i]}; });
    }

    // [x1, x2) x [-inf, y)
    int prefix_count(XY x1, XY x2, XY y) {
        return WM.prefix_count(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y);
    }
    // [x1, x2) x [y1, y2)
    int count(XY x1, XY x2, XY y1, XY y2) {
        return WM.count(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y1, y2);
    }
    // [x1, x2) x [-inf, y)
    pair<int, T> prefix_count_and_prod(XY x1, XY x2, XY y) {
        return WM.prefix_count_and_prod(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y);
    }
    // [x1, x2) x [y1, y2)
    pair<int, T> count_and_prod(XY x1, XY x2, XY y1, XY y2) {
        return WM.count_and_prod(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y1, y2);
    }
    // [x1, x2) x [-inf, inf)
    T prod_all(XY x1, XY x2) { return WM.prod_all(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2)); }
    // [x1, x2) x [-inf, y)
    T prefix_prod(XY x1, XY x2, XY y) {
        return WM.prefix_prod(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y);
    }
    // [x1, x2) x [y1, y2)
    T prod(XY x1, XY x2, XY y1, XY y2) {
        return WM.prod(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), y1, y2);
    }

    // i is the index of the point as passed to build
    void set(int i, T t) { WM.set(new_idx[i], t); }
    void multiply(int i, T t) { WM.multiply(new_idx[i], t); }
    void add(int i, T t) { WM.add(new_idx[i], t); }

    // largest y with check(y, cnt, prod) over [x1, x2) x [0, y) (uncompressed y only)
    template <typename F>
    tuple<XY, int, T> max_right(XY x1, XY x2, F check) {
        return WM.max_right(IDX_X.val_to_idx(x1), IDX_X.val_to_idx(x2), check);
    }
};
} // namespace mitsuha
#endif // AJAY_WAVELET_MATRIX_2D_RANGE
