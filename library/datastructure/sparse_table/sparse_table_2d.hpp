#ifndef AJAY_SPARSE_TABLE_2D
#define AJAY_SPARSE_TABLE_2D

#include "library/datastructure/sparse_table/sparse_table.hpp"
#include "library/datastructure/sparse_table/disjoint_sparse_table.hpp"

namespace mitsuha{
// Construction O(HW log W)
// Query O(log H)
template <typename SPARSE_TABLE>
struct Sparse_Table_2D {
    using ST = SPARSE_TABLE;
    using MX = typename ST::MX;
    using X = typename MX::value_type;
    using value_type = X;
    static_assert(MX::commute);
    int H, W;
    vector<ST> dat;

    Sparse_Table_2D() {}
    Sparse_Table_2D(vector<vector<X>> &v) {
        H = len(v), W = (H == 0 ? 0 : len(v[0]));
        dat.resize(2 * H);
        For(i, H) { dat[H + i] = ST(v[i]); }
        Frr(i, 1, H) {
            dat[i] = ST(W, [&](int j) -> X {
                X x = dat[2 * i + 0].prod(j, j + 1);
                X y = dat[2 * i + 1].prod(j, j + 1);
                return MX::op(x, y);
            });
        }
    }

    X prod(int xl, int xr, int yl, int yr) {
        assert(0 <= xl && xl <= xr && xr <= H);
        assert(0 <= yl && yl <= yr && yr <= W);
        X res = MX::unit();
        xl += H, xr += H;
        while (xl < xr) {
            if (xl & 1) res = MX::op(res, dat[xl++].prod(yl, yr));
            if (xr & 1) res = MX::op(res, dat[--xr].prod(yl, yr));
            xl >>= 1, xr >>= 1;
        }
        return res;
    }
};
} // namespace mitsuha
#endif // AJAY_SPARSE_TABLE_2D