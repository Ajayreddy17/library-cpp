#ifndef AJAY_AM_SUMMAX_ASSIGN
#define AJAY_AM_SUMMAX_ASSIGN

#include "library/monoid/monoid_summax.hpp"
#include "library/monoid/monoid_assign.hpp"

namespace mitsuha{
template <typename E, E none_val>
struct ActedMonoid_SumMax_Assign {
    using Monoid_X = Monoid_SumMax<E>;
    using Monoid_A = Monoid_Assign<E, none_val>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X& x, const A& a, const long long& size) {
        if (a == Monoid_A::unit()) return x;
        return {E(size) * a, a};
    }
};
} // namespace mitusha
#endif // AJAY_AM_SUMMAX_ASSIGN