#ifndef AJAY_TRIANGULAR_FENWICKTREE_2D
#define AJAY_TRIANGULAR_FENWICKTREE_2D

#include "library/datastructure/fenwick_tree/fenwicktree_2d.hpp"

namespace mitsuha{
template<typename Monoid, typename XY, bool SMALL_X = false>
struct Triangular_Fenwicktree_2D{
    using MX = Monoid;
    static_assert(MX::commute);
    using E = typename MX::value_type;

    FenwickTree_2D<Monoid, XY, SMALL_X> XA, XB, XC;
    XY H;
    XY INF;

    Triangular_Fenwicktree_2D() {}
    Triangular_Fenwicktree_2D(vector<XY>& X, vector<XY>& Y) { build(X, Y); }
    Triangular_Fenwicktree_2D(vector<XY>& X, vector<XY>& Y, vector<E> wt) { build(X, Y, wt); }

    void build(vector<XY> X, vector<XY> Y) {
        INF = numeric_limits<XY>::max() / 2;
        int N = len(X);
        vector<XY> Yb(N);
        vector<XY> Xc(N);
        vector<XY> Yc(N);

        H = *max_element(X.begin(), X.end()) + 1;

        XA = FenwickTree_2D<Monoid, XY, SMALL_X>(X, Y);
        For(i, N){
            XY x = X[i];
            XY y = Y[i];
            Yb[i] = x + y;
            Xc[i] = H - 1 - x;
            Yc[i] = H - 1 - x + y;
        }
        XB = FenwickTree_2D<Monoid, XY, SMALL_X>(X, Yb);
        XC = FenwickTree_2D<Monoid, XY, SMALL_X>(Xc, Yc);
    }

    void build(vector<XY> X, vector<XY> Y, vector<E> wt) {
        INF = numeric_limits<XY>::max() / 2;
        int N = len(X);
        vector<XY> Yb(N);
        vector<XY> Xc(N);
        vector<XY> Yc(N);

        H = *max_element(X.begin(), X.end()) + 1;

        XA = FenwickTree_2D<Monoid, XY, SMALL_X>(X, Y, wt);
        For(i, N){
            XY x = X[i];
            XY y = Y[i];
            Yb[i] = x + y;
            Xc[i] = H - 1 - x;
            Yc[i] = H - 1 - x + y;
        }
        XB = FenwickTree_2D<Monoid, XY, SMALL_X>(X, Yb, wt);
        XC = FenwickTree_2D<Monoid, XY, SMALL_X>(Xc, Yc, wt);
    }

    void add(XY x, XY y, E val) {
        XA.add(x, y, val);
        XB.add(x, x + y, val);
        XC.add(H - 1 - x, H - 1 - x + y, val);
    }

    E sum(int x1, int x2, int y1, int y2) {
        return XA.sum(x1, x2, y1, y2);
    }
    E prod(int x1, int x2, int y1, int y2){ return sum(x1, x2, y1, y2); }

    E lower_right(int x, int y, int k){
        return MX::op(XA.sum(x, x + k, y, INF),
                MX::inverse(XB.sum(x, x + k, x + y + k, INF)));
    }
    E upper_left(int x, int y, int k){
        return MX::op(XA.sum(x - k, x, -INF, y),
                MX::inverse(XB.sum(x - k, x, -INF, x + y - k - 1)));
    }
    E upper_right(int x, int y, int k){
        return MX::op(XA.sum(x - k, x, y, INF),
                    MX::inverse(XC.sum(H - x, H - x + k, H - x + y + k, INF)));
    }
    E lower_left(int x, int y, int k){
        return MX::op(XA.sum(x, x + k, -INF, y),
                    MX::inverse(XC.sum(H - x - k, H - x, -INF, H - x + y - k - 1)));
    }
};
} // namespace mitsuha
#endif // AJAY_TRIANGULAR_FENWICKTREE_2D