#ifndef AJAY_TRIANGULAR_FENWICKTREE_2D_DENSE
#define AJAY_TRIANGULAR_FENWICKTREE_2D_DENSE

#include "library/datastructure/fenwick_tree/fenwicktree_2d_dense.hpp"
#include "library/linear_algebra/transpose.hpp"

namespace mitsuha{
// O(min(H ^ 2 + HW), min(W ^ 2 + HW)) memory
template<class Monoid>
struct Triangular_Fenwicktree_2D_Dense{
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    bool transposed = false;

    FenwickTree_2D_Dense<Monoid> XA, XB, XC;
    int H, W;
    int INF;

    Triangular_Fenwicktree_2D_Dense() {}
    Triangular_Fenwicktree_2D_Dense(vector<vector<X>> &A) { build(A); }

    void build(vector<vector<X>> A) {
        INF = numeric_limits<int>::max() / 2;
        H = len(A);
        W = (H == 0 ? 0 : len(A[0]));
        if (H > W){
            swap(H, W); A = transpose(A);
            transposed = true;
        }
        XA = FenwickTree_2D_Dense<Monoid>(A);
        vector<vector<X>> B(H, vector<X>(H + W, MX::unit()));
        vector<vector<X>> C(H, vector<X>(H + W, MX::unit()));

        For(x, H) For(y, W) {
            B[x][x + y] = A[x][y];
            C[x][x + y] = A[H - 1 - x][y];
        }
        XB = FenwickTree_2D_Dense<Monoid>(B);
        XC = FenwickTree_2D_Dense<Monoid>(C);
    }

    void add(int x, int y, X val) {
        if (transposed) swap(x, y);
        XA.add(x, y, val);
        XB.add(x, x + y, val);
        XC.add(H - 1 - x, H - 1 - x + y, val);
    }

    X sum(int x1, int x2, int y1, int y2) {
        if (transposed){
            swap(x1, y1);
            swap(x2, y2);
        }
        return XA.sum(x1, x2, y1, y2);
    }
    X prod(int x1, int x2, int y1, int y2){ return sum(x1, x2, y1, y2); }

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
#endif // AJAY_TRIANGULAR_FENWICKTREE_2D_DENSE