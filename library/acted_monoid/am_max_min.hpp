#ifndef AJAY_AM_MAX_MIN
#define AJAY_AM_MAX_MIN

#include "library/monoid/monoid_max.hpp"
#include "library/monoid/monoid_min.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_Max_Min {
    using Monoid_X = Monoid_Max<E>;
    using Monoid_A = Monoid_Min<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const long long &size) {
        return min(x, a);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MAX_MIN