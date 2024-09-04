#ifndef AJAY_MONOID_ADD_CHMIN_CHMAX
#define AJAY_MONOID_ADD_CHMIN_CHMAX

namespace mitsuha{
// For composition of functions {y = x + a, y = max(x, a), y = min(x, a)}
template <typename E>
struct Monoid_Add_Chmin_Chmax {
    using value_type = tuple<E, E, E>;
    using X = value_type;

    static X op(X x, X y) {
        auto [a, b, c] = x;
        auto [d, e, f] = y;
        a = a + d;
        b = (b == (numeric_limits<E>::max() / 2) ? b : b + d);
        c = (c == (-numeric_limits<E>::max() / 2) ? c : c + d);
        b = min(b, e);
        c = max(min(c, e), f);
        return {a, b, c};
    }

    static E eval(X f, E x) {
        auto [a, b, c] = f;
        return max(min(x + a, b), c);
    }

    static X add(E a) { return {a, numeric_limits<E>::max() / 2, -numeric_limits<E>::max() / 2}; }
    static X chmin(E b) { return {0, b, -numeric_limits<E>::max() / 2}; }
    static X chmax(E c) { return {0, numeric_limits<E>::max() / 2, c}; }

    static constexpr X unit() { return {0, numeric_limits<E>::max() / 2, -numeric_limits<E>::max() / 2}; }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD_CHMIN_CHMAX