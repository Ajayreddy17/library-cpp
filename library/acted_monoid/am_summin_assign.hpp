#ifndef AJAY_AM_SUMMIN_ASSIGN
#define AJAY_AM_SUMMIN_ASSIGN

#include "library/monoid/monoid_summin.hpp"
#include "library/monoid/monoid_assign.hpp"

namespace mitsuha{
template <typename E, long long none_val>
struct ActedMonoid_SumMin_Assign {
    using Monoid_X = Monoid_SumMin<E>;
    using Monoid_A = Monoid_Assign<long long, none_val>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, const long long& size) {
        if (a == Monoid_A::unit()) return x;
        return {E(size) * a, a};
    }
};
} // namespace mitsuha
#endif // AJAY_AM_SUMMIN_ASSIGN