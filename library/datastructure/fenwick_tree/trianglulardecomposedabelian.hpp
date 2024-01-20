#ifndef AJAY_TRIANGULARDECOMPOSEDABELIAN
#define AJAY_TRIANGULARDECOMPOSEDABELIAN

#include "library/datastructure/fenwick_tree/fenwicktree_2d_dense.hpp"
#include "library/linear_algebra/transpose.hpp"

namespace mitsuha{
// O(min(H ^ 2 + HW), min(W ^ 2 + HW)) memory
template<class Monoid>
struct TriangularDecomposedAbelian{
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    bool transposed = false;

    FenwickTree_2D_Dense<Monoid> XA, XB, XC;
    int H, W;

    TriangularDecomposedAbelian() {}
    TriangularDecomposedAbelian(vector<vector<X>> &A) { build(A); }

    void build(vector<vector<X>> &A) {
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
        return MX::op(XA.sum(x, x + k, y, numeric_limits<int>::max() / 2),
                MX::inverse(XB.sum(x, x + k, x + y + k, numeric_limits<int>::max() / 2)));
    }
    X upper_left(int x, int y, int k){
        if (transposed) swap(x, y);
        return MX::op(XA.sum(x - k, x, numeric_limits<int>::min() / 2, y),
                MX::inverse(XB.sum(x - k, x, numeric_limits<int>::min() / 2, x + y - k - 1)));
    }
    X upper_right(int x, int y, int k){
        if (transposed) {
            swap(x, y);
            return MX::op(XA.sum(x, x + k, numeric_limits<int>::min() / 2, y),
                    MX::inverse(XC.sum(H - x - k, H - x, numeric_limits<int>::min() / 2, H - x + y - k - 1)));
        }
        return MX::op(XA.sum(x - k, x, y, numeric_limits<int>::max() / 2),
                    MX::inverse(XC.sum(H - x, H - x + k, H - x + y + k, numeric_limits<int>::max() / 2)));
    }
    X lower_left(int x, int y, int k){
        if (transposed) {
            swap(x, y);
            return MX::op(XA.sum(x - k, x, y, numeric_limits<int>::max() / 2),
                    MX::inverse(XC.sum(H - x, H - x + k, H - x + y + k, numeric_limits<int>::max() / 2)));
        }
        return MX::op(XA.sum(x, x + k, numeric_limits<int>::min() / 2, y),
                    MX::inverse(XC.sum(H - x - k, H - x, numeric_limits<int>::min() / 2, H - x + y - k - 1)));
    }
    X lower_triangle(int x, int y, int k){ return lower_left(x, y, k) + lower_right(x, y, k); }
    X upper_triangle(int x, int y, int k){ return upper_left(x, y, k) + upper_right(x, y, k); }
    X left_triangle(int x, int y, int k){ return upper_left(x, y, k) + lower_left(x, y, k); }
    X right_triangle(int x, int y, int k){ return upper_right(x, y, k) + lower_right(x, y, k); }
    X tilted_square(int x, int y, int k){ return lower_triangle(x, y, k) + upper_triangle(x, y, k); }
};
} // namespace mitsuha
#endif // AJAY_TRIANGULARDECOMPOSEDABELIAN