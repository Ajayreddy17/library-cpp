#ifndef AJAY_MONOID_OR
#define AJAY_MONOID_OR

namespace mitsuha{
template <typename X>
struct Monoid_Or {
    using value_type = X;
    static X op(X x, X y) { return x | y; }
    static constexpr X unit() { return X(0); };
    static constexpr bool commute = true;
};
} // namespace mitsuha
#endif // AJAY_MONOID_OR