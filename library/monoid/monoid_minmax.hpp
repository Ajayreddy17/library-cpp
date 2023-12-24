#ifndef AJAY_MONOID_MINMAX
#define AJAY_MONOID_MINMAX

namespace mitsuha{
template <class X>
struct Monoid_MinMax {
    using P = pair<X, X>;
    using value_type = P;
    static constexpr P op(const P x, const P y) noexcept {
        return {min(x.first, y.first), max(x.second, y.second)};
    }
    static constexpr P from_element(const X x) { return {x, x}; }
    static constexpr P unit() { 
        return {numeric_limits<X>::max() / 2, numeric_limits<X>::min() / 2}; 
    }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MINMAX