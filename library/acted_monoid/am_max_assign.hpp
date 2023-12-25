#ifndef AJAY_AM_MAX_ASSIGN
#define AJAY_AM_MAX_ASSIGN

#include "library/monoid/monoid_max.hpp"
#include "library/monoid/monoid_assign.hpp"

namespace mitsuha{
template <typename E, E none_val>
struct ActedMonoid_Max_Assign {
    using Monoid_X = Monoid_Max<E>;
    using Monoid_A = Monoid_Assign<E, none_val>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return (a == none_val ? x : a);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MAX_ASSIGN