#ifndef AJAY_MONOID_XOR
#define AJAY_MONOID_XOR

namespace mitsuha{
template <typename X>
struct Monoid_Xor {
    using value_type = X;
    static X op(X x, X y) { return x ^ y; }
    static constexpr X inverse(const X &x) noexcept { return x; }
    static constexpr X power(const X &x, long long n) noexcept {
        return (n & 1 ? x : 0);
    }
    static constexpr X unit(){return X(0);};
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_XOR