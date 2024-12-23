#ifndef AJAY_DUAL_FENWICKTREE
#define AJAY_DUAL_FENWICKTREE

#include "library/monoid/monoid_add.hpp"

namespace mitsuha{
template <typename Monoid>
struct Dual_FenwickTree {
    using G = Monoid;
    using E = typename G::value_type;
    int n;
    vector<E> dat;

    Dual_FenwickTree() {}
    Dual_FenwickTree(int n) { build(n); }

    void build(int m) {
        n = m;
        dat.assign(m, G::unit());
    }

    E operator[](int k) {
        E x = G::unit();
        for (++k; k <= n; k += k & -k) x = G::op(x, dat[k - 1]);
        return x;
    }
    E get(int k) {
        return (*this)[k];
    }
    vector<E> get_all() {
        vector<E> A = dat;
        for (int i = len(A); i >= 1; --i){
            int j = i + (i & -i);
            if (j <= len(A)) A[i - 1] += A[j - 1];
        }
        return A;
    }

    void apply(int L, int R, E x) {
        assert(0 <= L && L <= R && R <= n);
        E neg_x = G::inverse(x);
        while (L < R) { dat[R - 1] = G::op(x, dat[R - 1]), R -= R & -R; };
        while (R < L) { dat[L - 1] = G::op(neg_x, dat[L - 1]), L -= L & -L; };
    }
};

template<class Monoid>
std::ostream &operator<<(std::ostream &out, const Dual_FenwickTree<Monoid> &_ft){
    auto ft = _ft;
    out << "[";
    for(auto i = 0; i < ft.n; ++ i){
        out << ft[i];
        if(i != ft.n - 1) out << ", ";
    }
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_DUAL_FENWICKTREE