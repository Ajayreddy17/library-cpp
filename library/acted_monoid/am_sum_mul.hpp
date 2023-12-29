#ifndef AJAY_AM_SUM_MUL
#define AJAY_AM_SUM_MUL

#include "library/monoid/monoid_add.hpp"
#include "library/monoid/monoid_mul.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_Sum_Mul {
    using Monoid_X = Monoid_Add<E>;
    using Monoid_A = Monoid_Mul<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return x * a;
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUM_MUL