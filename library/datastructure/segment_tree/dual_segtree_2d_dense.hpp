#ifndef AJAY_DUAL_SEGTREE_2D_DENSE
#define AJAY_DUAL_SEGTREE_2D_DENSE

namespace mitsuha{
template <class Monoid>
struct Dual_SegTree_2D_Dense {
    using MX = Monoid;
    using X = typename MX::value_type;
    using value_type = X;
    static_assert(MX::commute);
    int H, W;
    vector<X> dat;

    Dual_SegTree_2D_Dense() : Dual_SegTree_2D_Dense(0, 0) {}
    Dual_SegTree_2D_Dense(int H, int W) : H(H), W(W), dat(4 * H * W, MX::unit()) {}

    X get(int x, int y) {
        X t = MX::unit();
        int a = x + H;
        while (a) {
            int b = y + W;
            while (b) { t = MX::op(t, dat[idx(a, b)]), b /= 2; }
            a /= 2;
        };
        return t;
    }

    vector<vector<X>> get_all() {
        For(x, 1, H) { For(y, W) push_x(x, y); }
        For(x, 1, H) {
            For(y, 2 * W) {
                dat[idx(2 * x + 0, y)] = MX::op(dat[idx(2 * x + 0, y)], dat[idx(x, y)]);
                dat[idx(2 * x + 1, y)] = MX::op(dat[idx(2 * x + 1, y)], dat[idx(x, y)]);
                dat[idx(x, y)] = MX::unit();
            }
        }
        vector<vector<X>> res(H, vector<X>(W, MX::unit()));
        For(x, H) For(y, W) { res[x][y] = dat[idx(x + H, y + W)]; }
        return res;
    }

    void apply(int xl, int xr, int yl, int yr, X x) {
        assert(0 <= xl && xl <= xr && xr <= H);
        assert(0 <= yl && yl <= yr && yr <= W);
        xl += H, xr += H;
        while (xl < xr) {
            if (xl & 1) apply_x(xl++, yl, yr, x);
            if (xr & 1) apply_x(--xr, yl, yr, x);
            xl >>= 1, xr >>= 1;
        }
    }

private:
    inline int idx(int x, int y) { return x * 2 * W + y; }
    
    void apply_x(int x, int yl, int yr, const X& t) {
        yl += W, yr += W;
        while (yl < yr) {
            if (yl & 1) { dat[idx(x, yl)] = MX::op(dat[idx(x, yl)], t), yl++; }
            if (yr & 1) { --yr, dat[idx(x, yr)] = MX::op(dat[idx(x, yr)], t); }
            yl >>= 1, yr >>= 1;
        }
    }

    void push_x(int x, int k) {
        dat[idx(x, 2 * k + 0)] = MX::op(dat[idx(x, 2 * k + 0)], dat[idx(x, k)]);
        dat[idx(x, 2 * k + 1)] = MX::op(dat[idx(x, 2 * k + 1)], dat[idx(x, k)]);
        dat[idx(x, k)] = MX::unit();
    }
};
} // namespace mitsuha
#endif // AJAY_DUAL_SEGTREE_2D_DENSE
