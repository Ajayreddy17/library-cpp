#ifndef AJAY_DUAL_CUMSUM_2D
#define AJAY_DUAL_CUMSUM_2D

namespace mitsuha{
template <typename Monoid>
struct Dual_Cumsum_2D {
    using MX = Monoid;
    static_assert(MX::commute);
    using X = typename MX::value_type;
    int H, W;
    vector<X> dat;
    bool built = false;

    Dual_Cumsum_2D() {}
    Dual_Cumsum_2D(int H, int W) : H(H), W(W), dat(H * W, MX::unit()) {}

    // [x1,x2) x [y1,y2)
    void add(int x1, int x2, int y1, int y2, X val) {
        assert(!built);
        assert(0 <= x1 && x1 <= x2 && x2 <= H);
        assert(0 <= y1 && y1 <= y2 && y2 <= W);
        if (x1 == x2 || y1 == y2) return;
        X neg = MX::inverse(val);
        dat[W * x1 + y1] = MX::op(dat[W * x1 + y1], val);
        if (y2 < W) dat[W * x1 + y2] = MX::op(dat[W * x1 + y2], neg);
        if (x2 < H) dat[W * x2 + y1] = MX::op(dat[W * x2 + y1], neg);
        if (x2 < H && y2 < W) dat[W * x2 + y2] = MX::op(dat[W * x2 + y2], val);
    }

    void build() {
        assert(!built);
        built = true;
        for (int i = 0; i < H * W; i++) {
            if (i % W != 0) dat[i] = MX::op(dat[i - 1], dat[i]);
        }
        for (int i = W; i < H * W; i++) dat[i] = MX::op(dat[i - W], dat[i]);
    }

    X get(int x, int y) const {
        assert(built);
        assert(0 <= x && x < H && 0 <= y && y < W);
        return dat[W * x + y];
    }
};
} // namespace mitsuha
#endif // AJAY_DUAL_CUMSUM_2D
