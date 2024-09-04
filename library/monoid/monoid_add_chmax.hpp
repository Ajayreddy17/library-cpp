#ifndef AJAY_MONOID_ADD_CHMAX
#define AJAY_MONOID_ADD_CHMAX

namespace mitsuha{
// For composition of functions {y = x + a, y = max(x, a)}
template <typename E>
struct Monoid_Add_Chmax {
    using value_type = pair<E, E>;
    using X = value_type;
    
    static X op(X x, X y) {
        auto [a, c] = x;
        auto [d, f] = y;
        a = a + d;
        c = (c == (-numeric_limits<E>::max() / 2) ? c : c + d);
        c = max(c, f);
        return {a, c};
    }

    static E eval(X f, E x) { return max(x + f.first, f.second); }
    
    static X add(E a) { return {a, (-numeric_limits<E>::max() / 2)}; }
    static X chmax(E c) { return {0, c}; }

    static constexpr X unit() { return {0, (-numeric_limits<E>::max() / 2)}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD_CHMAX