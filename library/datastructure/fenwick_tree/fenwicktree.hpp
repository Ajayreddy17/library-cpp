#ifndef AJAY_FENWICK_TREE
#define AJAY_FENWICK_TREE

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename Monoid>
struct FenwickTree {
    using G = Monoid;
    using MX = Monoid;
    using E = typename G::value_type;
    int n;

    FenwickTree() {}
    FenwickTree(int n) { build(n); }
    template <typename F>
    FenwickTree(int n, F f) {
        build(n, f);
    }
    FenwickTree(const vector<E>& v) { build(v); }

    E prod_all() const { return total; }
    E sum_all() const { return total; }
    E sum(int k) { return prod(k); }
    E prod(int k) const { return prefix_prod(k); }
    E prefix_prod(int k) const {
        chmin(k, n);
        E ret = G::unit();
        for (; k > 0; k -= k & -k) ret = G::op(ret, dat[k - 1]);
        return ret;
    }
    E prefix_sum(int k) const { return prefix_prod(k); }
    E sum(int L, int R) { return prod(L, R); }
    E prod(int L, int R) const {
        chmax(L, 0), chmin(R, n);
        if (R <= L) return G::unit();
        if (L == 0) return prefix_prod(R);
        assert(0 <= L && L <= R && R <= n);
        E pos = G::unit(), neg = G::unit();
        while (L < R) { pos = G::op(pos, dat[R - 1]), R -= R & -R; }
        while (R < L) { neg = G::op(neg, dat[L - 1]), L -= L & -L; }
        return G::op(pos, G::inverse(neg));
    }

    void add(int k, E x) { multiply(k, x); }
    void multiply(int k, E x) {
        static_assert(G::commute);
        total = G::op(total, x);
        for (++k; k <= n; k += k & -k) dat[k - 1] = G::op(dat[k - 1], x);
    }

    auto operator[](int i) {
        assert(i >= 0 and i < n);
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

    vector<E> get_all() {
        vector<E> res(n);
        For(i, n) res[i] = prod(i, i + 1);
        return res;
    }

    // use only when check is monotonic,
    // i.e using monoid_add all vals >= 0
    template <class F>
    int max_right(int L, const F& check) {
        assert(check(G::unit()));
        E s = G::unit();
        int i = L;
        int k = [&]() {
            while (1) {
                if (i % 2 == 1) { s = G::op(s, G::inverse(dat[i - 1])), i -= 1; }
                if (i == 0) { return (n == 0 ? -1 : 31 - __builtin_clz(n)) + 1; }
                int k = (i == 0 ? -1 : __builtin_ctz(i)) - 1;
                if (i + (1 << k) > n) return k;
                E t = G::op(s, dat[i + (1 << k) - 1]);
                if (!check(t)) { return k; }
                s = G::op(s, G::inverse(dat[i - 1])), i -= i & -i;
            }
        }();
        while (k) {
            --k;
            if (i + (1 << k) - 1 < len(dat)) {
                E t = G::op(s, dat[i + (1 << k) - 1]);
                if (check(t)) { i += (1 << k), s = t; }
            }
        }
        return i;
    }

    // check(i, x)
    // use only when check is monotonic,
    // i.e using monoid_add all vals >= 0
    template <class F>
    int max_right_with_index(int L, const F& check) {
        assert(check(L, G::unit()));
        E s = G::unit();
        int i = L;
        int k = [&]() {
            while (1) {
                if (i % 2 == 1) { s = G::op(s, G::inverse(dat[i - 1])), i -= 1; }
                if (i == 0) { return (n == 0 ? -1 : 31 - __builtin_clz(n)) + 1; }
                int k = (i == 0 ? -1 : __builtin_ctz(i)) - 1;
                if (i + (1 << k) > n) return k;
                E t = G::op(s, dat[i + (1 << k) - 1]);
                if (!check(i + (1 << k), t)) { return k; }
                s = G::op(s, G::inverse(dat[i - 1])), i -= i & -i;
            }
        }();
        while (k) {
            --k;
            if (i + (1 << k) - 1 < len(dat)) {
                E t = G::op(s, dat[i + (1 << k) - 1]);
                if (check(i + (1 << k), t)) { i += (1 << k), s = t; }
            }
        }
        return i;
    }

    // use only when check is monotonic,
    // i.e using monoid_add all vals >= 0
    template <class F>
    int min_left(int R, const F& check) {
        assert(check(G::unit()));
        E s = G::unit();
        int i = R;
        int k = 0;
        while (i > 0 && check(s)) {
            s = G::op(s, dat[i - 1]);
            k = (i == 0 ? -1 : __builtin_ctz(i));
            i -= i & -i;
        }
        if (check(s)) {
            assert(i == 0);
            return 0;
        }
        while (k) {
            --k;
            E t = G::op(s, G::inverse(dat[i + (1 << k) - 1]));
            if (!check(t)) { i += (1 << k), s = t; }
        }
        return i + 1;
    }

    // access by index k in fenwick set
    int kth(E k, int L = 0) {
        return max_right(L, [&k](E x) -> bool { return x <= k; });
    }

private:
    vector<E> dat;
    E total;
    
public:
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
};

template<class Monoid>
std::ostream &operator<<(std::ostream &out, const FenwickTree<Monoid> &_ft){
    auto ft = _ft;
    out << "[";
    for(auto i = 0; i < ft.n; ++ i){
        out << ft[i];
        if(i != ft.n - 1) out << ", ";
    }
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_FENWICK_TREE