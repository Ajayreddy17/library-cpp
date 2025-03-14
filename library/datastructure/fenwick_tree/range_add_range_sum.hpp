#ifndef AJAY_RANGE_ADD_RANGE_SUM
#define AJAY_RANGE_ADD_RANGE_SUM

#include "library/datastructure/fenwick_tree/fenwicktree.hpp"

namespace mitsuha{
template <typename Monoid>
struct Range_Add_Range_Sum {
    using MX = Monoid;
    using E = typename MX::value_type;

    struct Mono {
        using value_type = pair<E, E>;
        using X = value_type;
        static X op(X x, X y) { 
            return make_pair(MX::op(x.first, y.first), MX::op(x.second, y.second)); 
        }
        static constexpr X unit() { 
            return make_pair(MX::unit(), MX::unit()); 
        }
        static constexpr bool commute = true;
    };
    FenwickTree<Mono> bit;

    Range_Add_Range_Sum() {}
    Range_Add_Range_Sum(int n) { build(n); }
    template <typename F>
    Range_Add_Range_Sum(int n, F f) {
        build(n, f);
    }
    Range_Add_Range_Sum(const vector<E>& v) { build(v); }

    void build(int m) {
        build(m, [](int i) -> E { return MX::unit(); });
    }
    void build(const vector<E>& v) {
        build(len(v), [&](int i) -> E { return v[i]; });
    }
    template <typename F>
    void build(int m, F f) {
        bit.build(m, [&](int i) -> pair<E, E> { return make_pair(f(i), MX::unit()); });
    }

    void add(int L, int R, E a) {
        E b = MX::inverse(a);
        bit.add(L, {MX::power(b, L), a});
        bit.add(R, {MX::power(a, R), b});
    }
    void apply(int L, int R, E a) { add(L, R, a); }

    E prod(int L, int R) {
        auto [x0, x1] = bit.sum(L);
        auto [y0, y1] = bit.sum(R);
        E x = MX::op(MX::power(x1, L), x0);
        E y = MX::op(MX::power(y1, R), y0);
        return MX::op(MX::inverse(x), y);
    }

    E sum(int L, int R){ return prod(L, R); }

    E get(int i){
        assert(i >= 0 and i < bit.n);
        return prod(i, i + 1);
    }
    vector<E> get_all(){
        vector<E> ret(bit.n);
        For(i, bit.n) ret[i] = get(i);
        return ret;
    }
};

template<typename Monoid>
ostream &operator<<(ostream &out, const Range_Add_Range_Sum<Monoid> &_ft){
    auto ft = _ft;
    out << "[";
    for(auto i = 0; i < ft.bit.n; ++ i){
        out << ft.prod(i, i + 1);
        if(i != ft.bit.n - 1) out << ", ";
    }
    return out << ']';
}
} // namespace mitsuha
#endif // AJAY_RANGE_ADD_RANGE_SUM