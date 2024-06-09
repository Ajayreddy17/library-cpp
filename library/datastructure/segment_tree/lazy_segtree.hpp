#ifndef AJAY_LAZY_SEGMENT_TREE
#define AJAY_LAZY_SEGMENT_TREE

#include "library/util/update_proxy_object.hpp"

namespace mitsuha{
template <typename ActedMonoid>
struct Lazy_SegTree {
    using AM = ActedMonoid;
    using MX = typename AM::Monoid_X;
    using MA = typename AM::Monoid_A;
    using X = typename MX::value_type;
    using A = typename MA::value_type;
    int n, log, size;
    vector<X> dat;
    vector<A> laz;

    Lazy_SegTree() {}
    Lazy_SegTree(int n) { build(n); }
    template <typename F>
    Lazy_SegTree(int n, F f) {
        build(n, f);
    }
    Lazy_SegTree(const vector<X>& v) { build(v); }

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
        for(int i = 0; i < n; ++i) dat[size + i] = f(i);
        for(int i = size - 1; i >= 1; --i) update(i);
    }

    auto operator[](int p) {
        assert(0 <= p and p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return UpdateProxyObject{ dat[p], [this, p] { update_from(p); } };
    }

    X get(int p) { return (*this)[p]; }
    vector<X> get_all() {
        vector<X> ret(n);
        for (int i = 0; i < n; ++i){
            ret[i] = get(i);
        }
        return ret;
    }

    void set(int p, X v) { (*this)[p] = v; }

    X operator()(int l, int r) {
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

    X prod(int l, int r) { return (*this)(l, r); }
    X prefix_prod(int r) { return (*this)(0, r); }
    X suffix_prod(int l) { return (*this)(l, n); }
    X all_prod() const { return dat[1]; }

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

    template <typename F>
    int max_right(int l, const F &check) {
        assert(0 <= l && l <= n);
        assert(check(MX::unit()));
        if (l == n) return n;
        l += size;
        for (int i = log; i >= 1; i--) push(l >> i);
        X sm = MX::unit();
        do {
            while (l % 2 == 0) l >>= 1;
            if (!check(MX::op(sm, dat[l]))) {
                while (l < size) {
                    push(l);
                    l = (2 * l);
                    if (check(MX::op(sm, dat[l]))) { sm = MX::op(sm, dat[l++]); }
                }
                return l - size;
            }
            sm = MX::op(sm, dat[l++]);
        } while ((l & -l) != l);
        return n;
    }

    template <typename F>
    int min_left(int r, const F &check) {
        assert(0 <= r && r <= n);
        assert(check(MX::unit()));
        if (r == 0) return 0;
        r += size;
        for (int i = log; i >= 1; i--) push((r - 1) >> i);
        X sm = MX::unit();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (!check(MX::op(dat[r], sm))) {
                while (r < size) {
                    push(r);
                    r = (2 * r + 1);
                    if (check(MX::op(dat[r], sm))) { sm = MX::op(dat[r--], sm); }
                }
                return r + 1 - size;
            }
            sm = MX::op(dat[r], sm);
        } while ((r & -r) != r);
        return 0;
    }

private:
    inline int topbit(int x) { 
        return (x == 0 ? -1 : 31 - __builtin_clz(x)); 
    }
    void update(int k) {
        dat[k] = MX::op(dat[2 * k], dat[2 * k + 1]);
    }
    void update_from(int p) {
        for (int i = 1; i <= log; i++) update(p >> i);
    }
    void apply_at(int k, A a) {
        long long sz = 1 << (log - topbit(k));
        dat[k] = AM::act(dat[k], a, sz);
        if (k < size) laz[k] = MA::op(laz[k], a);
    }
    void push(int k) {
        if (laz[k] == MA::unit()) return;
        apply_at(2 * k, laz[k]), apply_at(2 * k + 1, laz[k]);
        laz[k] = MA::unit();
    }
};

template<class ActedMonoid>
std::ostream &operator<<(std::ostream &out, const Lazy_SegTree<ActedMonoid> &_lseg){
    auto lseg = _lseg;
    return out << lseg.get_all();
}
} // namespace mitsuha
#endif // AJAY_LAZY_SEGMENT_TREE
