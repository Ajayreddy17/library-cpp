#ifndef AJAY_AM_MAX_MAX
#define AJAY_AM_MAX_MAX

#include "library/monoid/monoid_max.hpp"

namespace mitsuha{
template <typename E>
struct ActedMonoid_Max_Max {
    using Monoid_X = Monoid_Max<E>;
    using Monoid_A = Monoid_Max<E>;
    using X = typename Monoid_X::value_type;
    using A = typename Monoid_A::value_type;
    static constexpr X act(const X &x, const A &a, const ll &size) {
        return max(x, a);
    }
};
} // namespace mitsuha
#endif // AJAY_AM_MAX_MAX