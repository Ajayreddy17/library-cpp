#ifndef AJAY_RECTANGLE_UNION
#define AJAY_RECTANGLE_UNION

#include "library/datastructure/segment_tree/lazy_segtree.hpp"
#include "library/acted_monoid/am_minmincnt_add.hpp"

namespace mitsuha{
template <typename XY = int>
struct Rectangle_Union {
    using RECT = tuple<XY, XY, XY, XY>;
    vector<RECT> rectangles;
    vector<XY> X, Y;

    void add_rect(XY xl, XY xr, XY yl, XY yr) {
        assert(xl < xr && yl < yr);
        X.emplace_back(xl), X.emplace_back(xr), Y.emplace_back(yl), Y.emplace_back(yr);
        rectangles.emplace_back(xl, xr, yl, yr);
    }

    template <typename ANS_TYPE = long long>
    ANS_TYPE calc() {
        int N = len(X);
        auto ord_x = argsort(X);
        auto ord_y = argsort(Y);
        vector<int> rk_y(N);
        For(i, N) rk_y[ord_y[i]] = i;
        X = ange(X, ord_x);
        Y = ange(Y, ord_y);

        using AM = ActedMonoid_MinMincnt_Add<XY>;
        Lazy_SegTree<AM> seg(N - 1, [&](int i) -> pair<XY, XY> { return {0, Y[i + 1] - Y[i]}; });
        ANS_TYPE ANS = 0;
        XY total = Y.back() - Y[0];
        For(i, N - 1) {
            int k = ord_x[i] / 2;
            int a = (ord_x[i] & 1 ? -1 : 1);
            seg.apply(rk_y[2 * k], rk_y[2 * k + 1], a);
            auto [min, mincnt] = seg.prod_all();
            ANS_TYPE dy = total - (min == 0 ? mincnt : 0);
            ANS_TYPE dx = X[i + 1] - X[i];
            ANS += dx * dy;
        }
        return ANS;
    }
private:
    template <typename T>
    vector<T> ange(const vector<T> &A, const vector<int> &I) {
        vector<T> B(len(I));
        For(i, len(I)) B[i] = A[I[i]];
        return B;
    }
};
} // namespace mitsuha
#endif // AJAY_RECTANGLE_UNION