#ifndef AJAY_SEGTREE
#define AJAY_SEGTREE

#include "library/util/update_proxy_object.hpp"

namespace mitsuha{
template <class Monoid>
struct SegTree {
    using MX = Monoid;
    using T = typename MX::value_type;

    SegTree() : SegTree(0) {}
    SegTree(int n) { build(n); }
    template<class F> SegTree(int n, const F &f) { build(n, f); }
    SegTree(const std::vector<T> &a) {
        build(a.size(), [&](int i){ return a[i]; });
    }
    
    template<class F>
    void build(int _n, const F &f) {
        n = _n, m = ceil_pow2(_n);
        data.assign(m << 1, MX::unit());
        for (int i = 0; i < n; ++i) data[i + m] = f(i);
        for (int k = m - 1; k > 0; --k) update(k);
    }
    const T& get(int i) const {
        assert(0 <= i and i < n);
        return data[i + m];
    }
    T operator()(int l, int r) const {
        T res_l = MX::unit(), res_r = MX::unit();
        for (l += m, r += m; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res_l = MX::op(res_l, data[l++]);
            if (r & 1) res_r = MX::op(data[--r], res_r);
        }
        return MX::op(res_l, res_r);
    }
    T prod(int l, int r) const { return (*this)(l, r); }
    T prod_left(int r) const { return (*this)(0, r); }
    T prod_right(int l) const { return (*this)(l, m); }
    T prod_all() const { return data[1]; }

    void set(int i, const T &val) {
        (*this)[i] = val;
    }
    auto operator[](int i) {
        assert(0 <= i and i < n);
        int k = i + m;
        return UpdateProxyObject { data[k], [this, k]{ update_from(k); } };
    }

    template <typename F>
    int max_right(int l, const F &f) const {
        assert(0 <= l and l <= n);
        assert(f(MX::unit()));
        if (l == n) return n;
        l += m;
        T sum_l = MX::unit();
        do {
            while (l % 2 == 0) l >>= 1;
            if (not f(MX::op(sum_l, data[l]))) {
                while (l < m) {
                    l = 2 * l;
                    if (f(MX::op(sum_l, data[l]))) sum_l = MX::op(sum_l, data[l++]);
                }
                return l - m;
            }
            sum_l = MX::op(sum_l, data[l]);
            l++;
        } while ((l & -l) != l);
        return n;
    }

    template <class F>
    int min_left(int r, const F &f) const {
        assert(0 <= r && r <= n);
        assert(f(MX::unit()));
        if (r == 0) return 0;
        r += m;
        T sum_r = MX::unit();
        do {
            r--;
            while (r > 1 && (r % 2)) r >>= 1;
            if (not f(MX::op(data[r], sum_r))) {
                while (r < m) {
                    r = 2 * r + 1;
                    if (f(MX::op(data[r], sum_r))) sum_r = MX::op(data[r--], sum_r);
                }
                return r + 1 - m;
            }
            sum_r = MX::op(data[r], sum_r);
        } while ((r & -r) != r);
        return 0;
    }

    template<class output_stream>
    friend output_stream &operator<<(output_stream &out, SegTree<Monoid> &seg){
        out << "[";
        for(auto i = 0; i < seg.n; ++ i){
            out << seg.prod(i, i + 1);
            if(i != seg.n - 1) out << ", ";
        }
        return out << ']';
    }

private:
    int n, m;
    std::vector<T> data;

    static constexpr int ceil_pow2(int n) {
        int m = 1;
        while (m < n) m <<= 1;
        return m;
    }
    void update_from(int k) {
        for (k >>= 1; k; k >>= 1) update(k);
    }
    void update(int k) {
        data[k] = MX::op(data[k * 2], data[k * 2 + 1]);
    }
};
} // namespace mitsuha
#endif // AJAY_SEGMENT_TREE