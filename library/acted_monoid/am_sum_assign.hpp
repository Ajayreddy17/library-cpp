#ifndef AJAY_AM_SUM_ASSIGN
#define AJAY_AM_SUM_ASSIGN

#include "library/monoid/monoid_add.hpp"
#include "library/monoid/monoid_assign.hpp"

namespace mitsuha{
template <typename E, E none_val>
struct ActedMonoid_Sum_Assign {
    using Monoid_X = Monoid_Add<E>;
    using Monoid_A = Monoid_Assign<E, none_val>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return (a == Monoid_A::unit()) ? x: a * E(size);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUM_ASSIGN