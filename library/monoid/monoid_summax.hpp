#ifndef AJAY_MONOID_SUMMAX
#define AJAY_MONOID_SUMMAX

namespace mitsuha{
template <typename E>
struct Monoid_SumMax {
    using value_type = pair<E, E>;
    using X = value_type;
    static X op(X x, X y) { return {x.first + y.first, max(x.second, y.second)}; }
    static X from_element(E e) { return {e, e}; }
    static constexpr X unit() { return {E(0), numeric_limits<E>::min() / 2}; }
    static constexpr bool commute = 1;
};
} // namespace mitsuha
#endif // AJAY_MONOID_SUMMAX