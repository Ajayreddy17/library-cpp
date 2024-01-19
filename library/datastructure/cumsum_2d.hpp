#ifndef AJAY_CUMSUM_2D
#define AJAY_CUMSUM_2D

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename Monoid, bool default_allow_out_of_range = false>
struct Cumsum_2D {
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    int H, W;
    vector<X> dat;
 
    Cumsum_2D() {}
    Cumsum_2D(vector<vector<X>> &A) { build(A); }
 
    void build(vector<vector<X>> &A) {
        H = len(A);
        W = (H == 0 ? 0 : len(A[0]));
        dat.assign(H * W, MX::unit());
        for(int x = 0; x < H; x++) for(int y = 0; y < W; y++) {
            int k = W * x + y;
            dat[k] = (y == 0 ? A[x][y] : MX::op(dat[k - 1], A[x][y]));
        }
        for(int i = W; i < H * W; i++) dat[i] = MX::op(dat[i - W], dat[i]);
    }

    // [x1,x2) x [y1,y2)
    template <bool allow_out_of_range = default_allow_out_of_range>
    X sum(int x1, int x2, int y1, int y2) const{
        if constexpr (allow_out_of_range) {
            chmax(x1, 0), chmin(x2, H), chmax(y1, 0), chmin(y2, W);
            if (x1 >= x2 || y1 >= y2) return MX::unit();
        }
        assert(0 <= x1 && x1 <= x2 && x2 <= H);
        assert(0 <= y1 && y1 <= y2 && y2 <= W);
        if (x2 == 0 || y2 == 0) return MX::unit();
        --x1, --y1, --x2, --y2;
        X a = (x1 >= 0 && y1 >= 0 ? dat[W * x1 + y1] : MX::unit());
        X b = (x1 >= 0 && y2 >= 0 ? dat[W * x1 + y2] : MX::unit());
        X c = (x2 >= 0 && y1 >= 0 ? dat[W * x2 + y1] : MX::unit());
        X d = (x2 >= 0 && y2 >= 0 ? dat[W * x2 + y2] : MX::unit());
        return MX::op(MX::op(a, d), MX::inverse(MX::op(b, c)));
    }
 
    X prefix_sum(int x, int y) {
        return (x == 0 || y == 0) ? MX::unit() : dat[W * x + y - (W + 1)];
    }
};
} // namespace mitsuha
#endif // AJAY_CUMSUM_2D