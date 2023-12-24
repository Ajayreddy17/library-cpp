#ifndef AJAY_MONOID_MAX
#define AJAY_MONOID_MAX

namespace mitsuha{
template <typename E>
struct Monoid_Max {
    using X = E;
    using value_type = X;
    static constexpr X op(const X &x, const X &y) noexcept { return max(x, y); }
    static constexpr X unit() { return numeric_limits<E>::min() / 2; }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MAX