#ifndef AJAY_MONOID_ADD
#define AJAY_MONOID_ADD

namespace mitsuha{
template <typename X>
struct Monoid_Add {
    using value_type = X;
    static constexpr X op(const X &x, const X &y) noexcept { return x + y; }
    static constexpr X inverse(const X &x) noexcept { return -x; }
    static constexpr X power(const X &x, long long n) noexcept { return X(n) * x; }
    static constexpr X unit() { return X(0); }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ADD