#ifndef AJAY_Triangular_Sum_2D
#define AJAY_Triangular_Sum_2D

#include "library/util/cumsum_2d.hpp"
#include "library/linear_algebra/transpose.hpp"

namespace mitsuha{
// O(min(H ^ 2 + HW), min(W ^ 2 + HW)) memory
template<class Monoid>
struct Triangular_Sum_2D{
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    bool transposed;

    Cumsum_2D<Monoid, true> XA, XB, XC;
    int H, W;
    int INF;

    Triangular_Sum_2D() {}
    Triangular_Sum_2D(vector<vector<X>> &A) { build(A); }

    void build(vector<vector<X>> A) {
        H = len(A);
        W = (H == 0 ? 0 : len(A[0]));
        if (H > W){
            swap(H, W); A = transpose(A);
            transposed = true;
        } else transposed = false;
        INF = numeric_limits<int>::max() / 2;
        XA = Cumsum_2D<Monoid, true>(A);
        vector<vector<X>> B(H, vector<X>(H + W, MX::unit()));
        vector<vector<X>> C(H, vector<X>(H + W, MX::unit()));

        For(x, H) For(y, W) {
                B[x][x + y] = A[x][y];
                C[x][x + y] = A[H - 1 - x][y];
            }
        XB = Cumsum_2D<Monoid, true>(B);
        XC = Cumsum_2D<Monoid, true>(C);
    }

    X sum(int x1, int x2, int y1, int y2) const{
        if (transposed){
            swap(x1, y1);
            swap(x2, y2);
        }
        return XA.sum(x1, x2, y1, y2); 
    }
    X prod(int x1, int x2, int y1, int y2) const{ return sum(x1, x2, y1, y2); }

    X lower_right(int x, int y, int k){
        if (transposed) swap(x, y);
        return MX::op(XA.sum(x, x + k, y, INF),
                MX::inverse(XB.sum(x, x + k, x + y + k, INF)));
    }
    X upper_left(int x, int y, int k){
        if (transposed) swap(x, y);
        return MX::op(XA.sum(x - k, x, -INF, y),
                MX::inverse(XB.sum(x - k, x, -INF, x + y - k - 1)));
    }
    X upper_right(int x, int y, int k){
        if (transposed) {
            swap(x, y);
            return MX::op(XA.sum(x, x + k, -INF, y),
                    MX::inverse(XC.sum(H - x - k, H - x, -INF, H - x + y - k - 1)));
        }
        return MX::op(XA.sum(x - k, x, y, INF),
                    MX::inverse(XC.sum(H - x, H - x + k, H - x + y + k, INF)));
    }
    X lower_left(int x, int y, int k){
        if (transposed) {
            swap(x, y);
            return MX::op(XA.sum(x - k, x, y, INF),
                    MX::inverse(XC.sum(H - x, H - x + k, H - x + y + k, INF)));
        }
        return MX::op(XA.sum(x, x + k, -INF, y),
                    MX::inverse(XC.sum(H - x - k, H - x, -INF, H - x + y - k - 1)));
    }
};
} // namespace mitsuha
#endif // AJAY_Triangular_Sum_2D