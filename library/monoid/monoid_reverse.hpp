#ifndef AJAY_MONOID_REVERSE
#define AJAY_MONOID_REVERSE

namespace mitsuha{
// Source: https://github.com/maspypy/library/blob/main/alg/monoid/monoid_reverse.hpp
template <class Monoid>
struct Monoid_Reverse {
    using value_type = typename Monoid::value_type;
    using X = value_type;
    static constexpr X op(const X &x, const X &y) { return Monoid::op(y, x); }
    static constexpr X unit() { return Monoid::unit(); }
    static const bool commute = Monoid::commute;
};
} // namespace mitsuha
#endif // AJAY_MONOID_REVERSE
