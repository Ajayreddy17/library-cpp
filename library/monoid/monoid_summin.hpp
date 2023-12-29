#ifndef AJAY_MONOID_SUMMIN
#define AJAY_MONOID_SUMMIN

namespace mitsuha{
template <typename E>
struct Monoid_SumMin {
    using value_type = pair<E, E>;
    using X = value_type;
    static X op(X x, X y) { return {x.first + y.first, min(x.second, y.second)}; }
    static X from_element(E x) { return {x, x}; }
    static constexpr X unit() { return {E(0), numeric_limits<E>::max() / 2}; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_SUMMIN