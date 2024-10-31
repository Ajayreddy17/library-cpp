#ifndef AJAY_LICHAO
#define AJAY_LICHAO

namespace mitsuha{
// Evaluation score is long long
// Define T operator() for FUNC f, T is a comparable type
// Linear expression: FUNC = LiChaoTree_Line
template <typename FUNC, bool COMPRESS, bool MINIMIZE>
struct LiChao_Tree {
    using T = typename FUNC::value_type;
    vector<long long> X;
    long long lo, hi;
    vector<FUNC> dat;
    int n, log, size;

    inline int get_idx(long long x) {
        if constexpr (COMPRESS) { return lower_bound(X.begin(), X.end(), x) - X.begin(); }
        assert(lo <= x && x <= hi);
        return x - lo;
    }

    template <typename XY>
    LiChao_Tree(const vector<XY>& pts, FUNC default_fn) {
        static_assert(COMPRESS);
        for (auto&& x: pts) X.emplace_back(x);
        settify(X);
        n = len(X), log = 1;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        dat.assign(size << 1, default_fn);
    }

    LiChao_Tree(long long lo, long long hi, FUNC default_fn) : lo(lo), hi(hi) {
        static_assert(!COMPRESS);
        n = hi - lo, log = 1;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        dat.assign(size << 1, default_fn);
    }

    void add_line(FUNC f) { return add_line_at(1, f); }

    void add_segment(long long xl, long long xr, FUNC f) {
        xl = get_idx(xl), xr = get_idx(xr);
        xl += size, xr += size;
        while (xl < xr) {
            if (xl & 1) add_line_at(xl++, f);
            if (xr & 1) add_line_at(--xr, f);
            xl >>= 1, xr >>= 1;
        }
    }

    // Pair of optimal value and FUNC
    pair<T, FUNC> query(long long x) {
        FUNC f = dat[0]; // default_fn
        T fx = f(x);
        int i = get_idx(x) + size;
        while (i) {
            FUNC g = dat[i];
            T gx = g(x);
            if ((MINIMIZE && gx < fx) || (!MINIMIZE && gx > fx)) { f = g, fx = gx; }
            i >>= 1;
        }
        return {fx, f};
    }

    void add_line_at(int i, FUNC f) {
        int upper_bit = 31 - __builtin_clz(i);
        int l = (size >> upper_bit) * (i - (1 << upper_bit));
        int r = l + (size >> upper_bit);
        while (l < r) {
            FUNC g = dat[i];
            T fl = evaluate_inner(f, l), fr = evaluate_inner(f, r - 1);
            T gl = evaluate_inner(g, l), gr = evaluate_inner(g, r - 1);
            bool bl = (MINIMIZE ? fl < gl : fl > gl);
            bool br = (MINIMIZE ? fr < gr : fr > gr);
            if (bl && br) {
                dat[i] = f;
                return;
            }
            if (!bl && !br) return;
            int m = (l + r) / 2;
            T fm = evaluate_inner(f, m), gm = evaluate_inner(g, m);
            bool bm = (MINIMIZE ? fm < gm : fm > gm);
            if (bm) {
                dat[i] = f;
                f = g;
                if (!bl) { i = 2 * i + 0, r = m; }
                if (bl) { i = 2 * i + 1, l = m; }
            }
            if (!bm) {
                if (bl) { i = 2 * i + 0, r = m; }
                if (!bl) { i = 2 * i + 1, l = m; }
            }
        }
    }

private:
    inline T evaluate_inner(FUNC& f, long long x) {
        return f((COMPRESS ? X[min<int>(x, n - 1)] : min<int>(x + lo, hi - 1)));
    }
};
} // namespace mitsuha
#endif // AJAY_LICHAO
