#ifndef AJAY_MONOID_MIN
#define AJAY_MONOID_MIN

namespace mitsuha{
template <typename E>
struct Monoid_Min {
    using X = E;
    using value_type = X;
    static constexpr X op(const X &x, const X &y) noexcept { return min(x, y); }
    static constexpr X unit() { return numeric_limits<E>::max() / 2; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MIN