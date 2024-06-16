#ifndef AJAY_FENWICKTREE_RANGE_ADD
#define AJAY_FENWICKTREE_RANGE_ADD

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
// Can be faster than lazy seg tree
template <typename AbelGroup>
struct FenwickTree_Range_Add {
    using G = AbelGroup;
    using E = typename AbelGroup::value_type;
    int n;
    FenwickTree<G> bit0;
    FenwickTree<G> bit1;

    FenwickTree_Range_Add() {}
    FenwickTree_Range_Add(int n) { build(n); }
    template <typename F>
    FenwickTree_Range_Add(int n, F f) {
        build(n, f);
    }
    FenwickTree_Range_Add(const vector<E>& v) { build(v); }

    void build(int m) {
        n = m;
        bit0.build(n), bit1.build(n);
    }
    void build(const vector<E>& v) {
        build(len(v), [&](int i) -> E { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        n = m;
        bit0.build(m, f);
        bit1.build(m);
    }

    void add(int i, E val) { bit0.add(i, val); }

    void add(int L, int R, E val) {
        bit0.add(L, G::power(val, -L));
        bit0.add(R, G::power(val, R));
        bit1.add(L, val);
        bit1.add(R, G::inverse(val));
    }

    E prod(int L, int R) {
        E prod_R = G::op(G::power(bit1.prod(R), R), bit0.prod(R));
        E prod_L = G::op(G::power(bit1.prod(L), L), bit0.prod(L));
        return G::op(G::inverse(prod_L), prod_R);
    }

    E get(int i){
        assert(i >= 0 and i < n);
        return prod(i, i + 1);
    }
};

template <typename AbelGroup>
std::ostream &operator<<(std::ostream &out, const FenwickTree_Range_Add<AbelGroup> &_ft){
    auto ft = _ft;
    out << "[";
    for(auto i = 0; i < ft.n; ++ i){
        out << ft.prod(i, i + 1);
        if(i != ft.n - 1) out << ", ";
    }
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_FENWICKTREE_RANGE_ADD