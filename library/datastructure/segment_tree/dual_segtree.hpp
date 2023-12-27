#ifndef AJAY_DUAL_SEGTREE
#define AJAY_DUAL_SEGTREE

namespace mitsuha{
template <typename Monoid>
struct Dual_SegTree {
    using MA = Monoid;
    using A = typename MA::value_type;
    int n, log, size;
    vector<A> laz;

    Dual_SegTree() : Dual_SegTree(0) {}
    Dual_SegTree(int n) { build(n); }

    void build(int m) {
        n = m;
        log = 1;
        while ((1 << log) < n) ++log;
        size = 1 << log;
        laz.assign(size << 1, MA::unit());
    }

    A operator[](int p) {
        assert(0 <= p && p < n);
        p += size;
        for (int i = log; i >= 1; i--) push(p >> i);
        return laz[p];
    }

    A get(int p) {
        return (*this)[p];
    }

    vector<A> get_all() {
        for(int i = 0; i < size; i++) push(i);
        return {laz.begin() + size, laz.begin() + size + n};
    }

    void apply(int l, int r, const A& a) {
        assert(0 <= l && l <= r && r <= n);
        if (l == r) return;
        l += size, r += size;
        if (not MA::commute) {
            for (int i = log; i >= 1; i--) {
                if (((l >> i) << i) != l) push(l >> i);
                if (((r >> i) << i) != r) push((r - 1) >> i);
            }
        }
        while (l < r) {
            if (l & 1) all_apply(l++, a);
            if (r & 1) all_apply(--r, a);
            l >>= 1, r >>= 1;
        }
    }

private:
    void push(int k) {
        if (laz[k] == MA::unit()) return;
        all_apply(2 * k, laz[k]), all_apply(2 * k + 1, laz[k]);
        laz[k] = MA::unit();
    }
    void all_apply(int k, A a) { laz[k] = MA::op(laz[k], a); }
};

template<class Monoid>
std::ostream &operator<<(std::ostream &out, const Dual_SegTree<Monoid> &_seg){
    auto seg = _seg;
    out << "[";
    for(auto i = 0; i < seg.n; ++ i){
        out << seg[i];
        if(i != seg.n - 1) out << ", ";
    }
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_DUAL_SEGTREE