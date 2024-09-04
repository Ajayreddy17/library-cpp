#ifndef AJAY_MONOID_ADD_CHMIN
#define AJAY_MONOID_ADD_CHMIN

namespace mitsuha{
// For composition of functions {y = x + a, y = min(x, a)}
template <typename E>
struct Monoid_Add_Chmin {
    using value_type = pair<E, E>;
    using X = value_type;

    static X op(X x, X y) {
        auto [a, b] = x;
        auto [d, e] = y;
        a = a + d;
        b = (b == (numeric_limits<E>::max() / 2) ? b : b + d);
        b = min(b, e);
        return {a, b};
    }

    static E eval(X f, E x) { return min(x + f.first, f.second); }

    static X add(E a) { return {a, numeric_limits<E>::max() / 2}; }
    static X chmin(E b) { return {0, b}; }

    static constexpr X unit() { return {0, numeric_limits<E>::max() / 2}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD_CHMIN