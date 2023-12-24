#ifndef AJAY_MONOID_MUL
#define AJAY_MONOID_MUL

namespace mitsuha{
template <class T>
struct Monoid_Mul {
    using value_type = T;
    using X = T;
    static constexpr X op(const X &x, const X &y) noexcept { return x * y; }
    static constexpr X inverse(const X &x) noexcept { return X(1) / x; }
    static constexpr X unit() { return X(1); }
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_MUL