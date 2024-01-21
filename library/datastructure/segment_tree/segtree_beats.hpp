#ifndef AJAY_SEGTREE_BEATS
#define AJAY_SEGTREE_BEATS

namespace mitsuha{
template <typename ActedMonoid>
struct SegTree_Beats {
    using AM = ActedMonoid;
    using MX = typename AM::Monoid_X;
    using MA = typename AM::Monoid_A;
    using X = typename MX::value_type;
    using A = typename MA::value_type;
    int n, log, size;
    vector<X> dat;
    vector<A> laz;

    SegTree_Beats() {}
    SegTree_Beats(int n) { build(n); }
    template <typename F>
    SegTree_Beats(int n, F f) {
        build(n, f);
    }
    SegTree_Beats(const vector<X>& v) { build(v); }

    void build(int m) {
        build(m, [](int i) -> X { return MX::unit(); });
    }
    void build(const vector<X>& v) {
        build(len(v), [&](int i) -> X { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        n = m, log = 1;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        dat.assign(size << 1, MX::unit());
        laz.assign(size, MA::unit());
        for(int i = 0; i < n; i++) dat[size + i] = f(i);
        for(int i = size - 1; i >= 1; i--) update(i);
    }

    void update(int k) { dat[k] = MX::op(dat[2 * k], dat[2 * k + 1]); }
    void set(int p, X x) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        dat[p] = x;
        for (int i = 1; i <= log; i++) update(p >> i);
    }

    X get(int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return dat[p];
    }

    /*
    void all_apply(int k, A a) {
      dat[k] = ActedMonoid::act(dat[k], a);
      if (k < size) {
        laz[k] = MA::op(laz[k], a);
        if (dat[k].fail) push(k), update(k);
      }
    }
    */

    vector<X> get_all() {
        for(int k = 1; k < size; k++) { push(k); }
        return {dat.begin() + size, dat.begin() + size + n};
    }

    X prod(int l, int r) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return MX::unit();
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        X xl = MX::unit(), xr = MX::unit();
        while (l < r) {
            if (l & 1) xl = MX::op(xl, dat[l++]);
            if (r & 1) xr = MX::op(dat[--r], xr);
            l >>= 1, r >>= 1;
        }
        return MX::op(xl, xr);
    }

    X prod_all() { return dat[1]; }

    void apply(int l, int r, A a) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += size, r += size;
        for (int i = log; i >= 1; i--) {
            if (((l >> i) << i) != l) push(l >> i);
            if (((r >> i) << i) != r) push((r - 1) >> i);
        }
        int l2 = l, r2 = r;
        while (l < r) {
            if (l & 1) apply_at(l++, a);
            if (r & 1) apply_at(--r, a);
            l >>= 1, r >>= 1;
        }
        l = l2, r = r2;
        for (int i = 1; i <= log; i++) {
            if (((l >> i) << i) != l) update(l >> i);
            if (((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

private:
    void apply_at(int k, A a) {
        int sz = 1 << (log - (k == 0 ? -1 : 31 - __builtin_clz(k)));
        dat[k] = AM::act(dat[k], a, sz);
        if (k < size) {
            laz[k] = MA::op(laz[k], a);
            if (dat[k].fail) push(k), update(k);
        }
    }

    void push(int k) {
        if (laz[k] == MA::unit()) return;
        apply_at(2 * k, laz[k]), apply_at(2 * k + 1, laz[k]);
        laz[k] = MA::unit();
    }
};
} // namespace mitsuha
#endif // AJAY_SEGTREE_BEATS