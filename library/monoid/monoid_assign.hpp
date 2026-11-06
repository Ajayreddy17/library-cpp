#ifndef AJAY_MONOID_ASSIGN
#define AJAY_MONOID_ASSIGN

namespace mitsuha {
// Source: https://github.com/maspypy/library/blob/main/alg/monoid/assign.hpp
template <typename X, int none_val>
struct Monoid_Assign {
    using value_type = X;
    static X op(X x, X y) { return (y == X(none_val) ? x : y); }
    static constexpr X unit() { return X(none_val); }
    static constexpr bool commute = false;
};
} // namespace mitsuha
#endif // AJAY_MONOID_ASSIGN