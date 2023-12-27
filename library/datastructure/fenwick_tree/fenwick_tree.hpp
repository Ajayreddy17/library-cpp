#ifndef AJAY_FENWICK_TREE
#define AJAY_FENWICK_TREE

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename Monoid>
struct FenwickTree {
    using G = Monoid;
    using E = typename G::value_type;
    int n;
    vector<E> dat;
    E total;

    FenwickTree() {}
    FenwickTree(int n) { build(n); }
    template <typename F>
    FenwickTree(int n, F f) {
        build(n, f);
    }
    FenwickTree(const vector<E>& v) { build(v); }

    void build(int m) {
        n = m;
        dat.assign(m, G::unit());
        total = G::unit();
    }
    void build(const vector<E>& v) {
        build(len(v), [&](int i) -> E { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        n = m;
        dat.clear();
        dat.reserve(n);
        total = G::unit();
        for(int i = 0; i < n; ++i) { dat.emplace_back(f(i)); }
        for (int i = 1; i <= n; ++i) {
            int j = i + (i & -i);
            if (j <= n) dat[j - 1] = G::op(dat[i - 1], dat[j - 1]);
        }
        total = prefix_prod(m);
    }

    E prod_all() const { return total; }
    E prod(int k) const { return prefix_prod(k); }
    E prefix_prod(int k) const {
        chmin(k, n);
        E ret = G::unit();
        for (; k > 0; k -= k & -k) ret = G::op(ret, dat[k - 1]);
        return ret;
    }
    E prod(int L, int R) const {
        chmax(L, 0), chmin(R, n);
        if (L == 0) return prefix_prod(R);
        assert(0 <= L && L <= R && R <= n);
        E pos = G::unit(), neg = G::unit();
        while (L < R) { pos = G::op(pos, dat[R - 1]), R -= R & -R; }
        while (R < L) { neg = G::op(neg, dat[L - 1]), L -= L & -L; }
        return G::op(pos, G::inverse(neg));
    }

    void multiply(int k, E x) {
        static_assert(G::commute);
        total = G::op(total, x);
        for (++k; k <= n; k += k & -k) dat[k - 1] = G::op(dat[k - 1], x);
    }

    auto operator[](int i) {
        struct {
            int i;
            FenwickTree<Monoid>& ft;
            operator E() const { return ft.prod(i, i + 1); }
            auto& operator++() { return *this += 1; }
            auto& operator--() { return *this -= 1; }
            auto& operator+=(E val) { ft.multiply(i, val); return *this; }
            auto& operator-=(E val) { ft.multiply(i, -val); return *this; }
            auto& operator*=(E val) { E cur = ft.prod(i, i + 1); ft.multiply(i, cur * val - cur); return *this; }
            auto& operator/=(E val) { E cur = ft.prod(i, i + 1); ft.multiply(i, cur / val - cur); return *this; }
            auto& operator%=(E val) { E cur = ft.prod(i, i + 1); ft.multiply(i, cur % val - cur); return *this; }
            auto& operator =(E val) { E cur = ft.prod(i, i + 1); ft.multiply(i, val - cur); return *this; }
        } obj{ i, *this };
        return obj;
    }
    E operator()(int l, int r) const { return prod(l, r); }

    template <class F>
    int max_right(const F check) {
        assert(check(G::unit()));
        int i = 0;
        E s = G::unit();
        int k = 1;
        while (2 * k <= n) k *= 2;
        while (k) {
            if (i + k - 1 < len(dat)) {
                E t = G::op(s, dat[i + k - 1]);
                if (check(t)) { i += k, s = t; }
            }
            k >>= 1;
        }
        return i;
    }

    // check(i, x)
    template <class F>
    int max_right_with_index(const F check) {
        assert(check(0, G::unit()));
        int i = 0;
        E s = G::unit();
        int k = 1;
        while (2 * k <= n) k *= 2;
        while (k) {
            if (i + k - 1 < len(dat)) {
                E t = G::op(s, dat[i + k - 1]);
                if (check(i + k, t)) { i += k, s = t; }
            }
            k >>= 1;
        }
        return i;
    }

    int kth(E k) {
        return max_right([&k](E x) -> bool { return x <= k; });
    }

    template<class output_stream>
    friend output_stream &operator<<(output_stream &out, FenwickTree<Monoid> &ft){
        out << "[";
        for(auto i = 0; i < ft.n; ++ i){
            out << ft[i];
            if(i != ft.n - 1) out << ", ";
        }
        return out << ']';
    }
};
} // namespace mitsuha
#endif // AJAY_FENWICK_TREE